#pragma once

#include <pc/mem_map.hpp>
#include <pc/error.hpp>
#include <oracle/oracle.h>
#include <zlib.h>

namespace pc
{

  // replay pyth aggregate prices from capture file
  class replay : public error
  {
  public:

    replay();
    ~replay();

    // capture file
    void set_file( const std::string& cap_file );
    std::string get_file() const;

    // (re) initialize
    bool init();

    // time of price capture
    int64_t get_time() const;

    // account number
    pc_pub_key_t *get_account() const;

    // on-chain account capture
    pc_acc_t *get_update() const;

    // get next price capture
    bool get_next();

  private:

    struct hdr
    {
      int64_t      ts_;
      pc_pub_key_t key_;
      pc_acc_t     acc_;
    };

    hdr        *up_;
    char       *buf_;
    size_t      pos_;
    size_t      len_;
    gzFile      zfd_;
    std::string file_;
  };

  inline int64_t replay::get_time() const
  {
    return up_->ts_;
  }

  inline pc_pub_key_t *replay::get_account() const
  {
    return &up_->key_;
  }

  inline pc_acc_t *replay::get_update() const
  {
    return &up_->acc_;
  }

}
