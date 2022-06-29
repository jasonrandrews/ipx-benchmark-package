/*
** Copyright (c) 2019 Arm Limited. All rights reserved.
*/


#include "trickbox2.h"

trickbox2::trickbox2(sc_core::sc_module_name  name) :
  sc_module(name),
    bus("bus")
{

    // Register the blocking transport method
    bus.register_b_transport(this, &trickbox2::busReadWrite);

    // register the debug function
    bus.register_transport_dbg(this, &trickbox2::transport_dbg);

    // Clear regs
    memset(&regs, 0, sizeof(regs));

    clock_period = sc_core::sc_time(10, SC_NS);
}

void
#ifdef MP_SOCKET
trickbox2::busReadWrite(int tag, tlm::tlm_generic_payload &gp,
		         sc_core::sc_time         &delay)
#else
trickbox2::busReadWrite(tlm::tlm_generic_payload &gp,
		         sc_core::sc_time         &delay)
#endif
{
    // Break out the address, mask and data pointer. This should be only a
    // single byte access.
    sc_dt::uint64      addr    = gp.get_address();

    uint32_t          *dataPtr;
  
    int                offset;		// Data byte offset in word
    uint32_t           uaddr;		// address

    // Mask off the address to its range. This ought to have been done already
    // by an arbiter/decoder.
    uaddr = (uint32_t)(addr) & ADDR_MASK;
    offset = 0;

    dataPtr = reinterpret_cast<uint32_t*>(gp.get_data_ptr());

    // Which command?
    switch(gp.get_command()) 
    {
        case tlm::TLM_READ_COMMAND:  
            dataPtr[offset] = busRead(uaddr); 
            break;
        case tlm::TLM_WRITE_COMMAND: 
            busWrite(uaddr, dataPtr[offset]); 
            break;
        case tlm::TLM_IGNORE_COMMAND:
            gp.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
            break;
            return;
    }

    gp.set_response_status(tlm::TLM_OK_RESPONSE);
    delay += sc_core::sc_time(10, SC_NS);

}


uint32_t
trickbox2::busRead(uint32_t uaddr)
{
    uint32_t data = 0;
    sc_core::sc_time delay;

    switch(uaddr) 
    {
        case DR:
            data = regs.tbDR;
            break;
    }
  
    return (data);
}

void
trickbox2::simulation_finish()
{
    std::cout << "simulation is complete" << std::endl;
    sc_core::sc_stop();
}

void
trickbox2::busWrite(uint32_t uaddr, uint32_t wdata)
{
    sc_core::sc_time delay;


    switch (uaddr) 
    {
        case DR:
            regs.tbDR = wdata;
            if (wdata == 0xa)
                simulation_finish(); 
            else
                printf("%c",wdata);
            break;
    }
}

unsigned int
#ifdef MP_SOCKET
trickbox2::transport_dbg(int tag, tlm::tlm_generic_payload& gp)
#else
trickbox2::transport_dbg(tlm::tlm_generic_payload& gp)
#endif
{

    sc_dt::uint64      addr    = gp.get_address();
    uint32_t          *dataPtr;
    unsigned int      len = gp.get_data_length();

    int             offset;
    uint32_t        uaddr; 

    uaddr = (uint32_t) addr;
    offset = 0;

    dataPtr = reinterpret_cast<uint32_t*>(gp.get_data_ptr());

    switch (gp.get_command()) 
    {
        case tlm::TLM_READ_COMMAND:
             dataPtr[offset] = busRead(uaddr);
             break;
        case tlm::TLM_WRITE_COMMAND:
             busWrite(uaddr, dataPtr[offset]);
             break;
        case tlm::TLM_IGNORE_COMMAND:
            gp.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
            return (0);
    }

    gp.set_response_status(tlm::TLM_OK_RESPONSE);

    return (len);
}



