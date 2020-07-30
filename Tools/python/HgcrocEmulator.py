"""Configuration for HGCROC Emulator"""

class HgcrocEmulator() :
    """Configuration for HGCROC Emulator

    Attributes
    ----------
    pedestal : float
    clockCycle : float
    measTime : float
    timingJitter : float
    readoutPadCapacitance : float
    maxADCRange : float
    nADCs : float
    iSOI : float
    gain : float
    noiseRMS : float
    readoutThreshold : float
    toaThreshold : float
    totThreshold : float
    """

    def __init__(self) :

        #######################################################################
        # Settings of the chip
        self.pedestal = 50. #ADC counts - baseline factor to subtract off of readout
        self.clockCycle = 25.0 #ns
        self.measTime = 0. #ns
        self.timingJitter = self.clockCycle / 100. #ns - pretty arbitrarily chosen
        self.readoutPadCapacitance = 0.1 #pF <- derived from hardware of HGCROC
        self.maxADCRange = 320. #fC <- setting of HGCROC
        self.nADCs = 10 
        self.iSOI  = 0 
        self.rateUpSlope =  -0.345
        self.timeUpSlope = 70.6547
        self.rateDnSlope = 0.140068
        self.timeDnSlope = 87.7649
        self.timePeak    = 77.732

        #Voltage -> ADC Counts conversion
        # voltage [mV] / gain = ADC Counts
        #
        # gain = maximum ADC range [fC] ( 1 / readout pad capacitance in pF ) ( 1 / 2^10 ADC Counts ) = mV / ADC counts
        self.gain = self.maxADCRange/self.readoutPadCapacitance/1024 # mV / ADC

        #######################################################################
        # Physical Constants for Detector Materials

        self.noiseRMS         = 0. #mV - useless default
        self.setNoise( 700. , 25. ) #depends on readoutPadCapacitance

        self.readoutThreshold = 0. #mV - useless default
        self.toaThreshold     = 0. #mV - useless default
        self.totThreshold     = 0. #mV - useless default

    def calculateVoltage(self, electrons) :
        """Calculate the voltage signal [mV] of the input number of electrons

        Uses the charge of 1000 electrons in fC and the capacitance of the readout pads.

        electrons ( 0.162 fC / 1000 electrons ) ( 1 / capacitance in pF ) = voltage [mV]

        Parameters
        ----------
        electrons : int
            Number of electrons (or e-h pairs) produced
        """

        return electrons*(0.162/1000.)*(1./self.readoutPadCapacitance)
    
    def setNoise(self, noiseIntercept , noiseSlope ) :
        """Calculate the Noise RMS [mV] from the capacitance of the readout pads.

        Parameters
        ----------
        noiseIntercept : float
            Noise when there is no capacitance
        noiseSlope : float
            Ratio of noise in electrons to capacitance in pF of pads
        """

        self.noiseRMS = self.calculateVoltage(noiseIntercept + noiseSlope*self.readoutPadCapacitance)

    def setThresholdDefaults(self, nElectronsPerMIP):
        """Set the different thresholds of the chip

        The default calculation for the different thresholds if the following:
        - readout is 4 times the rms noise above the pedestal 
        - toa is 5 MIPs above the pedestal
        - tot is 50 MIPs above the pedestal

        These calculations depend on the following parameters;
        the user should call this function _after_ setting these parameters.
        - noiseRMS
        - pedestal
        - gain
        - readoutPadCapacitance

        Parameters
        ----------
        nElectronsPerMIP : int
            Number of electrons generated by a MIP in the detector
        """

        self.readoutThreshold = self.gain*self.pedestal + 4*self.noiseRMS
        self.toaThreshold = self.gain*self.pedestal + self.calculateVoltage( 5.*nElectronsPerMIP )
        self.totThreshold = self.gain*self.pedestal + self.calculateVoltage( 50.*nElectronsPerMIP )

