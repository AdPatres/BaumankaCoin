#include "./wallet.h"

using namespace ad_patres;
using namespace Botan;

#include <iostream>

std::vector<AddedOutput> Transaction::availibleTxes;
std::vector<Transaction> Block::nonValidated;

Wallet::Wallet() : faf(Botan::EC_Group("secp256k1")), thePrivateKey(Botan::ECDSA_PrivateKey(aga, faf)), encPrivateKey(Botan::PKCS8::BER_encode(thePrivateKey)), publicKey(thePrivateKey.subject_public_key()), m_miner(&m_server)
{
  setAvailibleForAdress();
  setCurrentSum();

  receiver.Initialize(publicKey);

  m_server.start();
}

Wallet::Wallet(secure_vector<uint8_t> priv, std::vector<uint8_t> pub)
: thePrivateKey(AlgorithmIdentifier(), priv), encPrivateKey(priv), publicKey(pub), m_miner(&m_server)
{
  receiver.Initialize(publicKey);
}

Wallet&
Wallet::operator=(const Wallet& oth)
{
  if (this != &oth)
    {
      encPrivateKey = oth.encPrivateKey;
      publicKey = oth.publicKey;
      faf = oth.faf;
      thePrivateKey = oth.thePrivateKey;
      address = oth.address;
    }
  
  return *this;
}

Wallet::~Wallet() { m_server.stop(); }
secure_vector<uint8_t>
Wallet::getAddress() const
{
  return address;
}

void
Wallet::changeMiner()
{
  if (m_miner.getState())
    m_miner.stop();
  else
    m_miner.start();
}

void
Wallet::setAvailibleForAdress()
{
  BlockchainMutex->lock_shared();
  availibleForAddress.clear();

  for (auto& i : Transaction::availibleTxes)
    {
      for (size_t j = 0; j < chain->blockChain[i.output.blockNumber]
                               .txs[i.output.txeNumber]
                               .tails.size();
           j++)
        if (chain->blockChain[i.output.blockNumber]
                .txs[i.output.txeNumber]
                .tails[j]
                .getInfo()
                .second
              == address
            && !i.usedTails[j])
          availibleForAddress.push_back(i);
    }

  BlockchainMutex->unlock_shared();
}

void
Wallet::welcome()
{
  std::cout << "choose variants" << std::endl;
  std::cout << "enter Wallet - print 1" << std::endl;
  std::cout << "create new Wallet - print 2" << std::endl;

  char c = ' ';
  std::cin >> c;
  switch (c)
    {
      case ('1'):
        {
          secure_vector<uint8_t> priv;
          std::vector<uint8_t> pub;
          for (size_t i = 0; i < 287; i++)
            {
              char info;
              std::cin >> info;
              priv.push_back(info);
            }
          for (size_t i = 0; i < 279; i++)
            {
              char info;
              std::cin >> info;
              pub.push_back(info);
            }
          *this = Wallet(priv, pub);
        }
      case ('2'):
        commandProg();
        break;
      default:
        std::cout << "you did smth wrong try again" << std::endl;
        welcome();
        break;
    }
}

void
Wallet::commandProg()
{
  std::cout << "Current Sum: " << sum << std::endl;
  std::cout << "choose variants" << std::endl;
  std::cout << "refresh Data - print 1" << std::endl;
  std::cout << "create new Transaction - print 2" << std::endl;
  std::cout << (m_miner.getState() ? "stop miner - print 3"
                                   : "run miner - print 3")
            << std::endl;

  char c = ' ';
  std::cin >> c;
  switch (c)
    {
      case ('1'):
        setAvailibleForAdress();
        setCurrentSum();
        commandProg();
        break;
      case ('2'):
        createTxe();
        commandProg();
        break;
      case ('3'):
        changeMiner();
        commandProg();
        break;
      default:
        std::cout << "you did smth wrong try again" << std::endl;
        welcome();
        break;
    }
}

void
Wallet::readInputs()
{
  uint8_t answer = 'y';
  while (answer == 'y')
    {
      size_t blockNum = 0;
      size_t txeNum = 0;
      size_t tail = 0;
      std::cout << "Enter block num \n";
      std::cin >> blockNum;
      std::cout << "Enter txe num \n";
      std::cin >> txeNum;
      std::cout << "Enter tail num \n";
      std::cin >> tail;
      receiver.addInput(Output(blockNum, txeNum), tail,
                        chain->blockChain[blockNum].txs[txeNum].getTxeData());
      std::cout << "add more inputs? y-yes, else - no";
      std::cin >> answer;
    }
}

void
Wallet::readTails()
{
  std::cout << "Now enter tails\n";
  uint8_t answer = 'y';
  while (answer == 'y')
    {
      std::cout << "First enter addr\n";
      secure_vector<uint8_t> addr;
      uint8_t input;
      while (input != '\n')
        {
          std::cin >> input;
          addr.push_back(input);
        }
      size_t money = 0;
      std::cout << "Now enter money to send on that addr";
      std::cin >> money;
      receiver.addTail(Tail(money, addr));
      std::cout << "add more tails? y-yes, else - no";
      std::cin >> answer;
    }
}

void
Wallet::createTxe()
{
  if (sum == 0)
    {
      std::cout << "you have no money" << std::endl;
      return;
    }
  receiver.clear();

  std::cout << "First add Inputs\n";
  BlockchainMutex->lock_shared();
  for (const auto i : availibleForAddress)
    {
      for (size_t j = 0; j < chain->blockChain[i.output.blockNumber]
                             .txs[i.output.txeNumber]
                             .tails.size();
           j++)
        if (chain->blockChain[i.output.blockNumber]
                .txs[i.output.txeNumber]
                .tails[j]
                .getInfo()
                .second
              == address
            && !i.usedTails[j])
          std::cout << "Block Number: " << i.output.blockNumber
                    << " Txe Number: " << i.output.txeNumber
                    << " Tail Number: " << j << " Ammount of money: "
                    << chain->blockChain[i.output.blockNumber]
                         .txs[i.output.txeNumber]
                         .tails[j]
                         .getInfo()
                         .first
                    << std::endl;
    }

  BlockchainMutex->lock_shared();
  readInputs();
  readTails();
  receiver.sign(thePrivateKey);
  auto tx = receiver.get();
  m_server.share(tx);
}

void
Wallet::setCurrentSum()
{
  BlockchainMutex->lock_shared();

  for (auto i : availibleForAddress)
    {
      for (size_t j = 0; j < chain->blockChain[i.output.blockNumber]
                             .txs[i.output.txeNumber]
                             .tails.size();
           j++)
        if (chain->blockChain[i.output.blockNumber]
                .txs[i.output.txeNumber]
                .tails[j]
                .getInfo()
                .second
              == address
            && !i.usedTails[j])
          sum += chain->blockChain[i.output.blockNumber]
                   .txs[i.output.txeNumber]
                   .tails[j]
                   .getInfo()
                   .first;
    }

  BlockchainMutex->unlock_shared();
}

std::vector<secure_vector<uint8_t>>
Wallet::getHashesAfter(uint64_t idx) const
{
  BlockchainMutex->lock_shared();

  ++idx;
  std::vector<secure_vector<uint8_t>> res(chain->blockChain.size() - idx);
  for (size_t i = idx, j = 0; i < chain->blockChain.size(); ++i, ++j)
    res[j] = SHA_256().process(chain->blockChain[i].getBlockData());

  BlockchainMutex->unlock_shared();
  return std::move(res);
}
