# ButterworthLP
Butterworth lowpass n-th order filter implementation using cascaded second-order sections (SOS).

Resulting SOS should be identical to the following Matlab design:<br />
<pre>
[z, p, k] = butter(order, cutoffFreq*2*pi, 'low', 's');
[zd, pd, kd] = bilinear(z, p, k, samplingFreq);
[sos, g] = zp2sos(zd, pd, kd);
</pre>
