/**
 * @file SiStripHit.h
 * @brief Class representing a Si sensor strip hit.
 * @author Omar Moreno, SLAC National Accelerator Laboratory
 */

#ifndef EVENT_SISTRIPHIT_H_
#define EVENT_SISTRIPHIT_H_

//----------------------//
//   C++ Standard Lib   //
//----------------------//
#include <iostream>
#include <string>

//----------//
//   ROOT   //
//----------//
#include "TRefArray.h"
#include "TObject.h"

//-------------//
//   LDMX-SW   //
//-------------//
#include "Event/RawHit.h"
#include "Event/SimTrackerHit.h"

namespace ldmx { 

    class SiStripHit : public RawHit { 
    
        public: 

            /** Constructor */
            SiStripHit(); 

            /** Destructor */
            virtual ~SiStripHit(); 

            /** 
             * Add a SimTrackerHit that contributes to this SiStripHit. 
             *
             * @param simTrackerHit A simulated tracker hit that contributes to
             *                      this SiStripHit.
             */
            void addSimTrackerHit(SimTrackerHit* simTrackerHit);

            /** 
             * Return a TRefArray with the SimTrackerHits associated with 
             * this SiStripHit.
             */ 
            TRefArray* getSimTrackerHits() { return simTrackerHits_; }

            /** Print a description of this object. */
            void Print(Option_t* option = "") const; 

            /** Reset this object. */
            void Clear(Option_t* option = "");
        
        private: 

            /** 
             * References to the SimTrackerHits used to create this 
             * SiStripHit.  
             */
            TRefArray* simTrackerHits_{new TRefArray{}};
            
            /** Class declaration */ 
            ClassDef(SiStripHit, 1);
            
    }; // SiStripHit
}

#endif // EVENT_SISTRIPHIT_H_
