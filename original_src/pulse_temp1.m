% Georegistration and Gaussing fitting of "In" waveforms to derive
% variables.
% To run the function type
% [num_o_peak,W_T,A_T] = pulse_temp1(in, out,Plot_Coord); 
% In - Incoming waveform matrix
% Out - Outgoing waveform matrix
% Plot_Coord - Coordinate matrix

function[num_o_peak,data_table] = pulse_temp1(in, out,Plot_Coord)
cord = Plot_Coord; % cordinates of first, last, anchor points of all waveforms

in_1 = in(:,(2:end));% Received waveforms (1st column has the index number and is removed hereafter)
out_1 = out(:,(2:end)); % Emitted waveforms (1st column has the index number and is removed hereafter)
max_in = max(in_1')'; %Maximums of each waveforms (This was to remove waveforms that has low amplitudes than the given threshold)
Wave_ID = in(:,1);
% 
idx = find(max_in<= 17); % Inddex of maximums less than or equal to 17 (Used 17 as that gives the noise threshold of 6 (1/e2))- cap value)
in(idx,:) = []; % removed all waveforms that has the maximum less than 17
out(idx,:) = []; % removed all respective outgoing waveforms that has the maximum less than 17
cord(idx,:) = []; % removed all corresponding coordinate rows has the maximum less than 17
Wave_ID(idx) = [];
[m,n] = size(in);
X = in(:,(2:end));
[M,I] = max(X');
M_low =[];
M_high = [];

for i = 1:length(I)
    M_low = [M_low,find(I<4)];
    M_high = [M_high,find(I>57)];
end
exclude = [M_low,M_high];
if length(exclude)>=1
    in(exclude,:) = []; % removed all waveforms that has the maximum less than 17
    out(exclude,:) = []; % removed all respective outgoing waveforms that has the maximum less than 17
    cord(exclude,:) = []; % removed all corresponding coordinate rows has the maximum less than 17
    Wave_ID(exclude) = [];
else
    in = in; % removed all waveforms that has the maximum less than 17
    out = out; % removed all respective outgoing waveforms that has the maximum less than 17
    cord = cord; % removed all corresponding coordinate rows has the maximum less than 17
    Wave_ID = Wave_ID;
end
     
    anchor = cord(:,[3,4,5]); % anchor coordinates
    samp = cord(:,[6,7,8]); % sample coordinates of received waveforms that is used to calculate the offset

    for i = 1:length(anchor)
        offset(i,:) = (samp(i,:)-anchor(i,:))/1000; % offset of each sample point
    end


    start_cord = cord(:,[9,10,11]); % first coorcinates of each received waveform
    end_cord = cord(:,[12,13,14]); % last coordinates of each received  waveforms
    dif_cord = start_cord - end_cord; % Coordinate difference

    n = [0,0,1]; % Normal vector (flat surface)

    offsetm = mean(offset,1); % mean of offest coordinates
    dotA_B = dot((offsetm)', (n')); % dot product between offset and the surface vector
    abs_dot = (sqrt(sum((offsetm).^2,2)) * sqrt(sum(n.^2,2)))'; % absolute of offset and surface vectors
    cos_alp= dotA_B/ abs_dot; % cos angle
    angle = 180-acosd(cos_alp); % Incident angle in degrees

    Cal = 9.660535682891269e-08; % calibration constant

    % creating strcuture format for each single waveform
    for i = 1: size(in,1)
        in_box.(sprintf('B_%d',i))= in(i,(2:end)); % each received waveform has a seperate strcuture
        out_box.(sprintf('B_%d',i))= out(i,(2:end)); % each emitted waveform has a seperate strcuture
        start_cord_box.(sprintf('B_%d',i)) = start_cord(i,:); % each starting coordinate row has a seperate strcuture
        end_cord_box.(sprintf('B_%d',i)) = end_cord(i,:); % each ending coordinate row has a seperate strcuture
%                 in_box.(sprintf('B_%d',i))(in_box.(sprintf('B_%d',i))<=0)= []; % remove all "in" columns that is less than or equal to "0".
        in_box.(sprintf('B_%d',i)) =in_box.(sprintf('B_%d',i))(1:find(in_box.(sprintf('B_%d',i)),1,'last')); % remove all trailing "in" columns that is less than or equal to "0".
        id_box.(sprintf('B_%d',i))= repmat((linspace(1,length(in_box.(sprintf('B_%d',i))),length(in_box.(sprintf('B_%d',i))))),3,1); % Gave a sequential ID to each received waveform bin value from 1 to whatever length it has. Made three rows to correspondent with coordinate X,Y,and Z
    %     diff_cord_box.(sprintf('B_%d',i)) = dif_cord(i,:);
        offset_box.(sprintf('B_%d',i)) = offset(i,:); % offest matrix in a structure
        offset_point.((sprintf('B_%d',i))) = id_box.(sprintf('B_%d',i)).*offset_box.(sprintf('B_%d',i))'; % multiply the offset value by the ID to find how many offests are off a perticular bin
        offset_rep.(sprintf('B_%d',i)) = repmat(offset_box.(sprintf('B_%d',i)),length(id_box.(sprintf('B_%d',i))),1); 
         % each starting coordinate row has a seperate strcuture 
        cord_amp.(sprintf('B_%d',i)) = (offset_point.((sprintf('B_%d',i)))+(repmat(start_cord_box.(sprintf('B_%d',i)),length(id_box.(sprintf('B_%d',i))),1))')'; % add offsets to starting coordinates to create the coordinates for each waveform bin
    end

% If there is more than one incoming waveform (as digitizer can record upto certain numbers, if the incoming waveform is long, it will truncate into segments) for a single emitting waveform we
% need to append all incoming waveforms of those outgoing waveforms together
% before further processing. The following code will find if there is any
% and try to append. If there is none, the code will not beak and continue
% with other records.

    try

        cord1 = Plot_Coord1;
        in_11 = in1(:,(2:end));% Received waveforms (1st column has the index number and is removed hereafter)
        out_11 = out1(:,(2:end)); % Emitted waveforms (1st column has the index number and is removed hereafter)
        max_in1 = max(in_11')'; %Maximums of each waveforms (This was to remove waveforms that has low amplitudes than the given threshold)
        Wave_ID1 = in1(:,1);
        % 
        idx1 = find(max_in1<= 17); % Inddex of maximums less than or equal to 17 (Used 17 as that gives the noise threshold of 6 (1/e2))- cap value)
        in1(idx1,:) = []; % removed all waveforms that has the maximum less than 17
        out1(idx1,:) = []; % removed all respective outgoing waveforms that has the maximum less than 17
        cord1(idx1,:) = []; % removed all corresponding coordinate rows has the maximum less than 17
        Wave_ID1(idx1) = [];
        
        end_cord1 = cord1(:,[12,13,14]);
        [match_in,in_0_ind,in_1_ind] = intersect(Wave_ID,Wave_ID1);
        anchor1 = cord1(:,[3,4,5]); % anchor coordinates
        samp1 = cord1(:,[6,7,8]); % sample coordinates of received waveforms that is used to calculate the offset

        for i = 1:length(anchor1)
            offset1(i,:) = (samp1(i,:)-anchor1(i,:))/1000; % offset of each sample point
        end
        for i = 1: size(in1,1)
            in1_box.(sprintf('B_%d',i))= in1(i,(2:end)); % each received waveform has a seperate strcuture
            end_cord1_box.(sprintf('B_%d',i)) = end_cord1(i,:); % each ending coordinate row has a seperate strcuture
%                 in_box.(sprintf('B_%d',i))(in_box.(sprintf('B_%d',i))<=0)= []; % remove all "in" columns that is less than or equal to "0".
            in1_box.(sprintf('B_%d',i)) =in1_box.(sprintf('B_%d',i))(1:find(in1_box.(sprintf('B_%d',i)),1,'last')); % remove all trailing "in" columns that is less than or equal to "0".
            id1_box.(sprintf('B_%d',i))= fliplr(repmat((linspace(1,length(in1_box.(sprintf('B_%d',i))),length(in1_box.(sprintf('B_%d',i))))),3,1)); % Gave a sequential ID to each received waveform bin value from 1 to whatever length it has. Made three rows to correspondent with coordinate X,Y,and Z
        %     diff_cord_box.(sprintf('B_%d',i)) = dif_cord(i,:);
            offset1_box.(sprintf('B_%d',i)) = offset1(i,:); % offest matrix in a structure
            offset1_point.((sprintf('B_%d',i))) = id1_box.(sprintf('B_%d',i)).*offset1_box.(sprintf('B_%d',i))'; % multiply the offset value by the ID to find how many offests are off a perticular bin
            offset1_rep.(sprintf('B_%d',i)) = repmat(offset1_box.(sprintf('B_%d',i)),length(id1_box.(sprintf('B_%d',i))),1); 
            cord1_amp.(sprintf('B_%d',i)) = ((offset1_point.((sprintf('B_%d',i)))*-1)+(repmat(end_cord1_box.(sprintf('B_%d',i)),length(id1_box.(sprintf('B_%d',i))),1))')'; % add offsets to starting coordinates to create the coordinates for each waveform bin
        end

    catch
        disp('No such file')
    end

    try
        for r = 1:length(in_0_ind)
           dif_cord_seg.(sprintf('B_%d',r)) = cord_amp.(sprintf('B_%d',in_0_ind(r)))(end,:) - cord1_amp.(sprintf('B_%d',in_1_ind(r)))(1,:);
           mis_seg_num.(sprintf('B_%d',r)) = round(abs(dif_cord_seg.(sprintf('B_%d',r))./offset1_box.(sprintf('B_%d',in_1_ind(r)))));
           in_box.(sprintf('B_%d',in_0_ind(r))) = [in_box.(sprintf('B_%d',in_0_ind(r))),zeros(1,mis_seg_num.(sprintf('B_%d',(r)))(1)),in1_box.(sprintf('B_%d',in_1_ind(r)))]+1;
           cord_amp.(sprintf('B_%d',in_0_ind(r))) = [cord_amp.(sprintf('B_%d',in_0_ind(r)));repmat(cord1_amp.(sprintf('B_%d',in_1_ind(r)))(1,:),mis_seg_num.(sprintf('B_%d',(r)))(1),1);cord1_amp.(sprintf('B_%d',in_1_ind(r)))];
        end
    catch
        disp('No, in1 file')
    end

    n = [0,0,-1];
    n_for_all = repmat(n,size(in,1),1);
    for i = 1: size(in,1)
        id_boxnw.(sprintf('B_%d',i))= repmat((linspace(1,length(in_box.(sprintf('B_%d',i))),length(in_box.(sprintf('B_%d',i))))),3,1); % Gave a sequential ID to each received waveform bin value from 1 to whatever length it has. Made three rows to correspondent with coordinate X,Y,and Z
        anchor_box.(sprintf('B_%d',i)) = anchor(i,:); % anchor coodinates in a structure
        [fit_wav.(sprintf('C_%d',i)),peaks.(sprintf('C_%d',i)),locs.(sprintf('I_%d',i)),width.(sprintf('C_%d',i))] = nlgfit(in_box.(sprintf('B_%d',i)),out_box.(sprintf('B_%d',i))); % find maximum, width (halfheight),peak locations of each received wavforms 
        num_peaks.(sprintf('C_%d',i)) = numel(peaks.(sprintf('C_%d',i))); % number of peaks
        id_peaks.(sprintf('B_%d',i))= repmat((linspace(1,length(peaks.(sprintf('C_%d',i))),length(peaks.(sprintf('C_%d',i))))),1,1);

    end
        
    % get data in strcuture to a matrix
 % number of peaks in each waveform
    n_peak =struct2cell(num_peaks);  
    num_o_peak = horzcat(n_peak{:});
    
    % Pulse ID within each waveform
    id_pulse =struct2cell(id_peaks);  
    index_peak = horzcat(id_pulse{:});
    
    % pulse width
    w_total = struct2cell(width);  
    width_pulse = horzcat(w_total{:}); 
    % amplitude
    A_total = struct2cell(peaks);  
    Amp_pulse = horzcat(A_total{:}); 
    
    Head = {'pulse_ID','Pulse width','Amplitude'}; % data headers
    data = [index_peak',width_pulse',Amp_pulse']; % extracted variables
    data_table = dataset({data,Head{:}}); % data in a table
    
end
