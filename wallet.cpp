#include "./wallet.h"

using namespace Botan;

std::vector<AddedOutput> Transaction::availibleTxes;
std::vector<Transaction> Block::nonValidated;

Wallet::Wallet() : m_miner(&m_server)
{
  AutoSeeded_RNG rng;
  encPrivateKey = PKCS8::BER_encode(thePrivateKey); // rng, "default"
  publicKey = thePrivateKey.subject_public_key();
  address = SHA_256().process(publicKey);

  setAvailibleForAdress();
  setCurrentSum();

  receiver.Initialize(publicKey);
  chain = Blockchain::instance();

  m_server.start();
}

Wallet::Wallet(secure_vector<byte> priv, std::vector<byte> pub)
: encPrivateKey(priv), publicKey(pub),
  thePrivateKey(AlgorithmIdentifier(), encPrivateKey), m_miner(&m_server)
{
  address = SHA_256().process(publicKey);
  receiver.Initialize(publicKey);
  chain = Blockchain::instance();

  // m_server.start();
}

Wallet&
Wallet::operator=(const Wallet& v2)
{
  encPrivateKey = v2.encPrivateKey;
  publicKey = v2.publicKey;
  faf = v2.faf;
  thePrivateKey = v2.thePrivateKey;
  address = v2.address;

  // m_server = v2.m_server;
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
          secure_vector<byte> priv;
          std::vector<byte> pub;
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
        createTxe(std::cin, std::cout);
        commandProg();
        break;
      case ('3'):
        changeMiner();
        commandProg();
      default:
        std::cout << "you did smth wrong try again" << std::endl;
        welcome();
        break;
    }
}

void
Wallet::readInputs(std::istream& is, std::ostream& os)
{
  uint8_t answer = 'y';
  while (answer == 'y')
    {
      size_t blockNum = 0;
      size_t txeNum = 0;
      size_t tail = 0;
      os << "Enter block num \n";
      is >> blockNum;
      os << "Enter txe num \n";
      is >> txeNum;
      os << "Enter tail num \n";
      is >> tail;
      receiver.addInput(Output(blockNum, txeNum), tail,
                        chain->blockChain[blockNum].txs[txeNum].getTxeData());
      os << "add more inputs? y-yes, else - no";
      is >> answer;
    }
}

void
Wallet::readTails(std::istream& is, std::ostream& os)
{
  os << "Now enter tails\n";
  uint8_t answer = 'y';
  while (answer == 'y')
    {
      os << "First enter addr\n";
      secure_vector<byte> addr;
      uint8_t input;
      while (input != '\n')
        {
          is >> input;
          addr.push_back(input);
        }
      size_t money = 0;
      os << "Now enter money to send on that addr";
      is >> money;
      receiver.addTail(Tail(money, addr));
      os << "add more tails? y-yes, else - no";
      is >> answer;
    }
}

void
Wallet::createTxe(std::istream& is, std::ostream& os)
{
  if (sum == 0)
    {
      std::cout << "you have no money" << std::endl;
      return;
    }
  receiver.clear();

  os << "First add Inputs\n";
  BlockchainMutex->lock_shared();
  for (const auto i : availibleForAddress)
    {
      for (auto j = 0; j < chain->blockChain[i.output.blockNumber]
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
          os << "Block Number: " << i.output.blockNumber
             << " Txe Number: " << i.output.txeNumber << " Tail Number: " << j
             << " Ammount of money: "
             << chain->blockChain[i.output.blockNumber]
                  .txs[i.output.txeNumber]
                  .tails[j]
                  .getInfo()
                  .first
             << std::endl;
    }

  BlockchainMutex->lock_shared();
  readInputs(is, os);
  readTails(is, os);
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
      for (auto j = 0; j < chain->blockChain[i.output.blockNumber]
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

std::vector<secure_vector<byte>>
Wallet::getHashesAfter(uint64_t idx) const
{
  BlockchainMutex->lock_shared();

  ++idx;
  std::vector<secure_vector<byte>> res(chain->blockChain.size() - idx);
  for (size_t i = idx, j = 0; i < chain->blockChain.size(); ++i, ++j)
    res[j] = SHA_256().process(chain->blockChain[i].getBlockData());

  BlockchainMutex->unlock_shared();
  return std::move(res);
}
