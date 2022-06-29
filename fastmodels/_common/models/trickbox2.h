/*
** Copyright (c) 2019 Arm Limited. All rights reserved.
*/

#ifndef TRICKBOX2_H
#define TRICKBOX2_H

//
// Trickbox2 to match Cycle Models CPAKs
// This is to match the Cortex-M33 software exit function
//

#include <iostream>
#ifdef _WIN32
  typedef unsigned __int32 uint32_t;
#else
  #include <stdint.h>
#endif

#ifndef SC_INCLUDE_DYNAMIC_PROCESSES
#define SC_INCLUDE_DYNAMIC_PROCESSES
#endif

#include "systemc.h"
#include "tlm.h"

#ifdef MP_SOCKET
#include "tlm_utils/multi_passthrough_target_socket.h"
#else
#include "tlm_utils/simple_target_socket.h"
#endif

#define ADDR_MASK 0xFFF

#define DR        0x000


class trickbox2
  : public sc_core::sc_module
{
public:

  trickbox2(sc_core::sc_module_name  name);

  // Simple target convenience socket for bus access to registers
#ifdef MP_SOCKET
  tlm_utils::multi_passthrough_target_socket<trickbox2, 64, tlm::tlm_base_protocol_types, 1, sc_core::SC_ONE_OR_MORE_BOUND>  bus;
#else
  tlm_utils::simple_target_socket<trickbox2, 64>  bus;
#endif

  // debug function
#ifdef MP_SOCKET
  unsigned int transport_dbg(int tag, tlm::tlm_generic_payload& gp);
#else
  unsigned int transport_dbg(tlm::tlm_generic_payload& gp);
#endif

public:


  // Blocking transport function. Split out separate read and write
  // functions. The busReadWrite() and busWrite() functions will be
  // reimplemented in derived classes, so are declared virtual. The
  // busRead() function is only used here, so declared private (below).
#ifdef MP_SOCKET
  virtual void   busReadWrite(int tag, tlm::tlm_generic_payload &payload,
			               sc_core::sc_time         &delay);
#else
  virtual void   busReadWrite(tlm::tlm_generic_payload &payload,
			      sc_core::sc_time         &delay);
#endif

  virtual void   busWrite(uint32_t  uaddr, uint32_t wdata);
			  

  // trickbox2 registers
  struct {
     uint32_t tbDR;      // 0x000;
  } regs;


private:
    sc_time  clock_period;

    void simulation_finish();

    uint32_t busRead(uint32_t uaddr);

    void DR_bus_read(
        uint32_t& data,
        sc_core::sc_time& delay);

    void DR_bus_write(
        uint32_t& data,
        sc_core::sc_time& delay);

};


#endif
