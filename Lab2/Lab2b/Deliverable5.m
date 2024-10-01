% Down the mountain
Vreal = .0461 + (9.51*(10^(-4)).*adc_rounded) + (-3.86*(10^(-8)).*(adc_rounded.^2)); % tf from lab 1b to get ADC back to voltage
plot(temp_c, Vreal)
biasback = Vreal-bias;
plot(temp_c, biasback)
gainback = biasback/gain;
plot(temp_c, gainback)
WBback = (WR.*(gainback+(5/2)))./((5/2)-gainback); % inverse of wheatstone bridge function
plot(temp_c, WBback)
PR3back = (WBback.*PR3)./(PR3-WBback); % taking out the 3-point resistor
plot(temp_c, PR3back)
%temp = linspace(288.15, 303.15, 4096); %create a 4096 bit vector in Kelvin for 15-30 C
%NLR = 1000*exp(3636*((1./temp)-(1/298.15))); %use beta model to align temps to values
betaback = (1084073.4)./(3636 + 298.15*log(0.001*PR3back));
plot(temp, betaback)
tempbias = temp(4096) - betaback(4096);
tempdtm = betaback + tempbias;
