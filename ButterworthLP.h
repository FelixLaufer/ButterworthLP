#include <vector>
#include <complex>

#ifdef DEBUG
    #include <iostream>
    #include <iomanip>
#endif

/**
Analog prototype filter is digitalized using bilinear transform and split up into cascaded second-order sections (SOS).
*/
class ButterworthLP
{
public:

    /**
    Generates a Butterworth lowpass filter with a given normalized cutoff frequency and filter order.
    @param normalizedCutoffFrequency   (0, 1)     Normalized cutoff frequency := cuttoffFreq / samplingFreq.
    @param filterOrder                 [1, inf]   Butterworth filter order.
    */
    ButterworthLP(const double normalizedCutoffFrequency, const size_t filterOrder);

    /**
    Generates a Butterworth lowpass filter of a given order with a given cutoff frequency [Hz] in respect of the data sampling frequency [Hz].
    @param samplingFrequency   [1, inf] [Hz]                     Sampling frequency of the data.
    @param cutoffFrequency     [1, samplingFrequency - 1] [Hz]   Cutoff frequency
    @param filterOrder         [1, inf]                          Butterworth filter order.
    */
    ButterworthLP(const double samplingFrequency, const double cutoffFrequency, const size_t filterOrder);

    /**
    Set the filter state to a steady state w.r.t. to the given input value (assuming a constant filter input for infinite time steps in the past;
    see also http://www.emt.tugraz.at/publications/diplomarbeiten/da_hoebenreich/node21.html).
    @param value    Desired steady state output value
    */
    void stepInitialization(const double value);

    /**
    Processes the input value online depending on the current filter state.
    @param input   [-inf, inf]   Input value
    @return        [-inf, inf]   Filter response
    */
    double process(const double input);

    /**
    Processes the input data offline.
    @param *input                                  Ptr to input data.
    @param *output                                 Ptr to output data.
    @param size                    [0, inf]        Length of data.
    @param initialConditionValue   [-inf, inf]     Initializes the filter state to a steady state w.r.t. the given initial value (see stepInitialization).
    @param forwardBackward         {true, false}   Eliminate phase delay by filtering twice: forward and backward.
                                                   Note that forward-backward filtering corresponds to filtering with a 2n-th order filter.
    */
    void filter(const double *input, double *output, const size_t size, const double initialConditionValue = 0.0, const bool forwardBackward = false);

    ~ButterworthLP()
    { }

private:

    class SOS
    {

    public:

        SOS(const double b0, const double b1, const double b2, const double a1, const double a2, const double gain) :
            _b0(b0), _b1(b1), _b2(b2), _a1(a1), _a2(a2), _gain(gain), _z1(0), _z2(0),
            _preCompStateSpaceOutputVec1(_b1 - _b0*_a1),
            _preCompStateSpaceOutputVec2(_b2 - _b0*_a2)
        {
            #ifdef DEBUG
                std::cout << std::fixed << std::setprecision(6);
                size_t w = 10;
                std::cout <<_b0 << std::setw(w) << _b1 << std::setw(w) << _b2 << std::setw(w) << 1.0 << std::setw(w) << _a1 << std::setw(w) << _a2 << std::endl;
            #endif
        }

        void safeRestoreState(double &z1, double &z2, const bool restore = false);

        void stepInitialization(const double value);

        double process(const double input);

        ~SOS()
        { }

    private:

        const double _b0, _b1, _b2, _a1, _a2, _gain;

        const double _preCompStateSpaceOutputVec1, _preCompStateSpaceOutputVec2;

        double _z1, _z2;

    };

    void addSOS(const SOS sos);

    bool coefficients(const double normalizedCutoffFrequency, const size_t filterOrder);

    size_t _numSOS;

    std::vector<SOS> _sosVec;

};

