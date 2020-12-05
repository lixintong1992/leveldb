#include <cassert>
#include "leveldb/write_batch.h"
#include <iostream>
#include "leveldb/db.h"
#include "test_app.h"

int main(int argc, char** argv) {
  std::cout << "lxt in main!!!" << test_num << std::endl;
  leveldb::DB* db;
  leveldb::Options options;
  options.create_if_missing = true;
  options.error_if_exists = false;

  std::cout << "========= START =========" << std::endl;
  leveldb::Status status = leveldb::DB::Open(options, "./testdb", &db);
  if (!status.ok()) {
      std::cerr << "Open status:" << status.ToString() << std::endl;
      return 1;
  } else {
      std::cout << "Open status:" << status.ToString() << std::endl;
  }

  std::string key1 = "key1", key2 = "key2", value = "value";

  /*read/write opertions*/
  status = db->Put(leveldb::WriteOptions(), key1, value);
  if (!status.ok()) {
    std::cerr << "Put key1 status:" << status.ToString() << std::endl;
  }
  status = db->Get(leveldb::ReadOptions(), key1, &value);
  if (status.ok()) {
    std::cout << "key1's value:" << value << std::endl;
    status = db->Put(leveldb::WriteOptions(), key2, value);
    status = db->Get(leveldb::ReadOptions(), key2, &value);
    if (status.ok()) {
      std::cout << "key2's value:" << value << std::endl;
    }
  } else {
    std::cerr << "Get status:" << status.ToString() << std::endl;
  }
  if (status.ok()) {
    status = db->Delete(leveldb::WriteOptions(), key1);
  }
  if (!status.ok()) {
    std::cerr << "Delete key1 status:" << status.ToString() << std::endl;
  }

  /*automic update*/
  status = db->Get(leveldb::ReadOptions(), key1, &value);    
  if (status.ok()) {
    leveldb::WriteBatch batch;
    batch.Delete(key1);
    batch.Put(key2, value);
    status = db->Write(leveldb::WriteOptions(), &batch);
    if (!status.ok()) {
      std::cerr << "Batch oprate status:" << status.ToString() << std::endl;
    }
  }

  /*Iteration*/
  leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    std::cout << it->key().ToString() << ":" << it->value().ToString() << std::endl;
  }
  assert (it->status().ok());
  delete it;

  /*slice*/
  leveldb::Slice slice;
  if (true) {
    std::string str = "test";
    slice = str;
    std::cout << "1str addr:" << &str << std::endl;
    std::cout << "str value:" << str << std::endl;
    std::string* pstr = &str;
    *pstr = "test2";
    std::cout << "2str addr:" << &str << std::endl;
    std::cout << "str value:" << str << std::endl;
  }
  std::cout << "lxt slice is:" << slice.ToString()<< std::endl;

  delete db;

  return 0;
}

