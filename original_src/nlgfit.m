% gaussian fitting of the waveforms to find amplitude, pulse width and
% number of pulses in each waveform

function[fit_wav,peaks,locs,width] = nlgfit(in,out) % inputs are incoming and outgoing waveforms
options = optimoptions('lsqnonlin','Display','none'); % disable the messages in command line space when run the code
% intialize the inputs
in = in; % incoming waveforms
out = out; % Outgoing waveforms
t = 1:1:length(in);   % length of hte incoming wavforms (contant length for all due to zero padding)

[po,lo,wo] = findpeaks(out,'MinPeakHeight',16,'WidthReference','halfheight'); % intial guess of the pulse width (wo) of the outgoing waveforms. This function is a matlab builtin function.
v1 = diff(diff(in)*-1); % second derivative of the incoming wavforms
M = max(in); % maximum of the each incoming waveforms
% v1 = diff(y);
sav_g = sgolayfilt(v1,5,11); % savitzky golay filtering the second derivative to remove additional noise

% initial maximum amplitude (peak) values and their locations of filtered second derivatives 
[pk,lc] = findpeaks(sav_g,(1:1:length(sav_g)));
% Find values above a given threshold (the threshold were selected based on known waveforms
ind =find(pk>=4);
% Peak locations were shifted by 2 to match the locations in the original waveform (as these locations were found in second
% derivative)
if length(ind)>=1
    ld1 = lc(ind)+2;
    peaks = in(ld1);
    width = (repmat(wo,length(peaks),1))';
    locs = ld1;
else
    [m_pk,I_pk] = max(pk);
    ld1 = lc(I_pk)+2;
    peaks = in(ld1);
    width = (repmat(wo,length(peaks),1))';
    locs = ld1;
end

% gaussian fitting
% Assigned to fit maximum of 7 peaks for each waveform.
if length(peaks)==1 % number of Gaussians to fit. This is defined by the number of peaks defined by the second derivative in the above.
    fun = @(x) x(1)*exp(-((t-x(2))/x(3)).^2)-in; % function to fit Gaussians
    lb = [peaks(1)-2,locs(1)-2,width(1)-2]; % lower boundary of Peaks, locations, and pulse widths
    ub = [peaks(1)+2,locs(1)+2,width(1)+2]; % upper boundary of Peaks, locations, and pulse widths
    x0 = [peaks(1),locs(1),width(1)]; % intial conditions of Peaks, locations, and pulse widths
    x = lsqnonlin(fun,x0,lb,ub,options); % Gaussian fitted waveform
    
   %  These conditions are same for all the pulses within each waveform 
elseif length(peaks)==2
    fun = @(x) x(1)*exp(-((t-x(2))/x(3)).^2)+x(4)*exp(-((t-x(5))/x(6)).^2)- in; 
    lb = [peaks(1)-2,locs(1)-2,width(1)-2,peaks(2)-2,locs(2)-2,width(2)-2];
    ub = [peaks(1)+2,locs(1)+2,width(1)+2,peaks(2)+2,locs(2)+2,width(2)+2];
    x0 = [peaks(1),locs(1),width(1),peaks(2),locs(2),width(2)];
    x = lsqnonlin(fun,x0,lb,ub,options);
 
elseif length(peaks)==3
    
    fun = @(x) x(1)*exp(-((t-x(2))/x(3)).^2)+x(4)*exp(-((t-x(5))/x(6)).^2)+x(7)*exp(-((t-x(8))/x(9)).^2) - in; 

    lb = [peaks(1)-2,locs(1)-2,width(1)-2,peaks(2)-2,locs(2)-2,width(2)-2,peaks(3)-2,locs(3)-2,width(3)-2];
    ub = [peaks(1)+2,locs(1)+2,width(1)+2,peaks(2)+2,locs(2)+2,width(2)+2,peaks(3)+2,locs(3)+2,width(3)+2];
    x0 = [peaks(1),locs(1),width(1),peaks(2),locs(2),width(2),peaks(3),locs(3),width(3)];
    x = lsqnonlin(fun,x0,lb,ub,options);
    
elseif length(peaks)==4
        fun = @(x) x(1)*exp(-((t-x(2))/x(3)).^2)+x(4)*exp(-((t-x(5))/x(6)).^2)+x(7)*exp(-((t-x(8))/x(9)).^2)+x(10)*exp(-((t-x(11))/x(12)).^2) - in; 

        lb = [peaks(1)-2,locs(1)-2,width(1)-2,peaks(2)-2,locs(2)-2,width(2)-2,peaks(3)-2,locs(3)-2,width(3)-2,peaks(4)-2,locs(4)-2,width(4)-2];
        ub = [peaks(1)+2,locs(1)+2,width(1)+2,peaks(2)+2,locs(2)+2,width(2)+2,peaks(3)+2,locs(3)+2,width(3)+2,peaks(4)+2,locs(4)+2,width(4)+2];
        x0 = [peaks(1),locs(1),width(1),peaks(2),locs(2),width(2),peaks(3),locs(3),width(3),peaks(4),locs(4),width(4)];
        x = lsqnonlin(fun,x0,lb,ub,options);
        
elseif length(peaks)==5
        fun = @(x) x(1)*exp(-((t-x(2))/x(3)).^2)+x(4)*exp(-((t-x(5))/x(6)).^2)+x(7)*exp(-((t-x(8))/x(9)).^2)+x(10)*exp(-((t-x(11))/x(12)).^2)+x(13)*exp(-((t-x(14))/x(15)).^2) - in; 

        lb = [peaks(1)-2,locs(1)-2,width(1)-2,peaks(2)-2,locs(2)-2,width(2)-2,peaks(3)-2,locs(3)-2,width(3)-2,peaks(4)-2,locs(4)-2,width(4)-2,peaks(5)-2,locs(5)-2,width(5)-2];
        ub = [peaks(1)+2,locs(1)+2,width(1)+2,peaks(2)+2,locs(2)+2,width(2)+2,peaks(3)+2,locs(3)+2,width(3)+2,peaks(4)+2,locs(4)+2,width(4)+2,peaks(5)+2,locs(5)+2,width(5)+2];
        x0 = [peaks(1),locs(1),width(1),peaks(2),locs(2),width(2),peaks(3),locs(3),width(3),peaks(4),locs(4),width(4),peaks(5),locs(5),width(5)];
        x = lsqnonlin(fun,x0,lb,ub,options);
        
elseif length(peaks)==6
        fun = @(x) x(1)*exp(-((t-x(2))/x(3)).^2)+x(4)*exp(-((t-x(5))/x(6)).^2)+x(7)*exp(-((t-x(8))/x(9)).^2)+x(10)*exp(-((t-x(11))/x(12)).^2)+x(13)*exp(-((t-x(14))/x(15)).^2)+x(16)*exp(-((t-x(17))/x(18)).^2) - in; 

        lb = [peaks(1)-2,locs(1)-2,width(1)-2,peaks(2)-2,locs(2)-2,width(2)-2,peaks(3)-2,locs(3)-2,width(3)-2,peaks(4)-2,locs(4)-2,width(4)-2,peaks(5)-2,locs(5)-2,width(5)-2,peaks(6)-2,locs(6)-2,width(6)-2];
        ub = [peaks(1)+2,locs(1)+2,width(1)+2,peaks(2)+2,locs(2)+2,width(2)+2,peaks(3)+2,locs(3)+2,width(3)+2,peaks(4)+2,locs(4)+2,width(4)+2,peaks(5)+2,locs(5)+2,width(5)+2,peaks(6)+2,locs(6)+2,width(6)+2];
        x0 = [peaks(1),locs(1),width(1),peaks(2),locs(2),width(2),peaks(3),locs(3),width(3),peaks(4),locs(4),width(4),peaks(5),locs(5),width(5),peaks(6),locs(6),width(6)];
        x = lsqnonlin(fun,x0,lb,ub,options);
       
else length(peaks)==7
        fun = @(x) x(1)*exp(-((t-x(2))/x(3)).^2)+x(4)*exp(-((t-x(5))/x(6)).^2)+x(7)*exp(-((t-x(8))/x(9)).^2)+x(10)*exp(-((t-x(11))/x(12)).^2)+x(13)*exp(-((t-x(14))/x(15)).^2)+x(16)*exp(-((t-x(17))/x(18)).^2)+x(19)*exp(-((t-x(20))/x(21)).^2) - in; 

        lb = [peaks(1)-2,locs(1)-2,width(1)-2,peaks(2)-2,locs(2)-2,width(2)-2,peaks(3)-2,locs(3)-2,width(3)-2,peaks(4)-2,locs(4)-2,width(4)-2,peaks(5)-2,locs(5)-2,width(5)-2,peaks(6)-2,locs(6)-2,width(6)-2,peaks(7)-2,locs(7)-2,width(7)-2];
        ub = [peaks(1)+2,locs(1)+2,width(1)+2,peaks(2)+2,locs(2)+2,width(2)+2,peaks(3)+2,locs(3)+2,width(3)+2,peaks(4)+2,locs(4)+2,width(4)+2,peaks(5)+2,locs(5)+2,width(5)+2,peaks(6)+2,locs(6)+2,width(6)+2,peaks(7)+2,locs(7)+2,width(7)+2];
        x0 = [peaks(1),locs(1),width(1),peaks(2),locs(2),width(2),peaks(3),locs(3),width(3),peaks(4),locs(4),width(4),peaks(5),locs(5),width(5),peaks(6),locs(6),width(6),peaks(7),locs(7),width(7)];
        x = lsqnonlin(fun,x0,lb,ub,options);
       
    
end
fit_wav = fun(x)+ in; % fitted waveforms for all the "In" waveforms
peaks = x(1:3:end); % peak values
locs = round(x(2:3:end)); % location values
width = x(3:3:end); % width values
end


