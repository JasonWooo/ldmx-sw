/**
 * @file TargetBremFilter.h
 * @class TargetBremFilter
 * @brief Class defining a UserActionPlugin that allows a user to filter out 
 *        events that don't result in a brem within the target.
 * @author Omar Moreno, SLAC National Accelerator Laboratory
 */

#ifndef BIASING_TARGETBREMFILTER_H
#define BIASING_TARGETBREMFILTER_H

//----------------//
//   C++ StdLib   //
//----------------//
#include <algorithm>

//------------//
//   Geant4   //
//------------//
#include "G4RunManager.hh"

/*~~~~~~~~~~~~~*/
/*   SimCore   */
/*~~~~~~~~~~~~~*/
#include "SimApplication/UserAction.h"

/*~~~~~~~~~~~~~~~*/
/*   Framework   */
/*~~~~~~~~~~~~~~~*/
#include "Framework/Parameters.h" 

namespace ldmx {

    class TargetBremFilter : public UserAction {

        public:

            /// Constructor
            TargetBremFilter(const std::string& name, Parameters& parameters);

            /// Destructor 
            ~TargetBremFilter();

            /**
             * Implement the stepping action which performs the target volume biasing.
             * @param step The Geant4 step.
             */
            void stepping(const G4Step* step) final override;

            /**
             * Method called at the end of every event.
             *
             * @param event Geant4 event object.
             */
            void EndOfEventAction(const G4Event*) final override;

            /**
             * Classify a new track which postpones track processing.
             * Track processing resumes normally if a target PN interaction occurred.
             * @param aTrack The Geant4 track.
             * @param currentTrackClass The current track classification.
             */
            G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack, 
                const G4ClassificationOfNewTrack& currentTrackClass) final override;

            /**
             *
             */
            static std::vector<G4Track*> getBremGammaList() { return bremGammaTracks_; }

            /**
             *
             */
            static void removeBremFromList(G4Track* track);


            /// Retrieve the type of actions this class defines
            std::vector< TYPE > getTypes() final override { 
                return { TYPE::EVENT, TYPE::STACKING, TYPE::STEPPING }; 
            } 

        private:
            
            static std::vector<G4Track*> bremGammaTracks_; 

            /// The volume that the filter will be applied to. 
            G4String volumeName_{"target_PV"};

            /// Recoil electron threshold.  
            double recoilEnergyThreshold_{1500}; // MeV

            /// Brem gamma energy treshold
            double bremEnergyThreshold_{0}; 

            /// Flag indicating if the recoil electron track should be killed
            bool killRecoilElectron_{false};


    }; // TargetBremFilter
}

#endif // BIASING_TARGETBREMFILTER_H
