/*
 * main.cpp - Example Cortex-M33 system with SystemC peripheral
 *
 * Copyright 2011-2019 ARM Limited.
 * All rights reserved.
 */

/* Includes */
#include <scx_evs_m33.h>
#include "trickbox.h"
#include "trickbox2.h"


/*
 * User's entry point.
 */
int sc_main(int argc , char * argv[]) {

    /*
     * Initialize simulation 
     */
    scx::scx_initialize("m33");

    /*
     * Components
     */
    scx_evs_m33  m33("m33");
    amba_pv::amba_pv_to_tlm_bridge<64> amba2tlm("amba2tlm");
    amba_pv::amba_pv_to_tlm_bridge<64> amba2tlm2("amba2tlm2");
    trickbox   trickbox("trickbox");
    trickbox2  trickbox2("trickbox2");
    
    /*
     * Simulation configuration
     */
   
    /* From command-line options */
    scx::scx_parse_and_configure(argc, argv);

    /* Simulation quantum, i.e. seconds to run per quantum */
    tlm::tlm_global_quantum::instance().set(sc_core::sc_time(1.0
                                                             / 100000000,
                                                             sc_core::SC_SEC));

    /* Simulation minimum synchronization latency */
    scx::scx_set_min_sync_latency(1.0 / 100000000);

    /*
     * Bindings
     */
    m33.amba_pv_m(amba2tlm.amba_pv_s);
    amba2tlm.tlm_m.bind(trickbox.bus);

    m33.amba_pv_m2(amba2tlm2.amba_pv_s);
    amba2tlm2.tlm_m.bind(trickbox2.bus);
    
    /*
     * Start of simulation
     */
    sc_core::sc_start();
    return EXIT_SUCCESS;
}