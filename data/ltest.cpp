#include "Account.h"
#include "Transaction.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdexcept>
class MTransaction : public Transaction {
 public:
  MTransaction() : Transaction() {}
  MOCK_METHOD(void, SaveToDataBase, (Account& from, Account& to, int sum), (override));
};

class MAccount : public Account {
 private:
  int id;
  int balance;
 public:
  MAccount(int id, int balance) : Account(id, balance) {}
  MOCK_METHOD(int, GetBalance, (), (const, override));
  MOCK_METHOD(void, ChangeBalance, (int), (override));
  MOCK_METHOD(void, Lock, (), (override));
  MOCK_METHOD(void, Unlock, (), (override));
};


TEST(Account, function) {
  Account aone(1, 1000);
  EXPECT_EQ(1000, aone.GetBalance());
  aone.Lock();
  aone.ChangeBalance(2000);
  aone.Unlock();
  EXPECT_EQ(3000, aone.GetBalance());
  try {
    aone.ChangeBalance(1);
  }
  catch (std::runtime_error& el) {}
  EXPECT_EQ(3000, aone.GetBalance());
}

TEST(Transaction, function) {
  Account aone(1, 10000);
  Account atwo(2, 10000);
  Transaction tr;
  Transaction tr2; tr2.set_fee(500);
  try {tr.Make(aone, aone, 100);}
  catch (std::logic_error& el) {}
  try {tr.Make(aone, atwo, -100);}
  catch (std::invalid_argument& el) {}
  try {tr.Make(aone, atwo, 0);}
  catch (std::logic_error& el) {}
  EXPECT_EQ(false, tr2.Make(aone, atwo, 200));
  tr.Make(aone, atwo, 1999);
  EXPECT_EQ(aone.GetBalance(), 10000); EXPECT_EQ(atwo.GetBalance(), 9999);
}
using ::testing::AtLeast;

TEST(Account, Mock) {
  MAccount aone(1, 1000);
  EXPECT_CALL(aone, GetBalance()).Times(AtLeast(1));
  std::cout <<  aone.GetBalance() << std::endl;
  EXPECT_CALL(aone, Lock()).Times(AtLeast(1));
  aone.Lock();
  EXPECT_CALL(aone, ChangeBalance(1)).Times(AtLeast(1));
  aone.ChangeBalance(1);
  EXPECT_CALL(aone, Unlock()).Times(AtLeast(1));
  aone.Unlock();
  
}

TEST(Transaction, Mock) {
  Account aone(1, 10000);
  Account atwo(2, 10000);
  MTransaction tr;
  EXPECT_CALL(tr, SaveToDataBase(aone, atwo, 1999)).Times(AtLeast(1));
  tr.Make(aone, atwo, 1999);
  
  
}

