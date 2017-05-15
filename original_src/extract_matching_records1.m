tic
% read a set of files that has In (140831_175352_1_WAVE_IN_0.txt), out (140831_175352_1_WAVE_OUT.txt) and coordinates (140831_175352_1_PULSE.csv) data 
in_files = 'H:\waveform_test\140831_175352_1_WAVE_IN_0.txt'; % in data file
out_files = 'H:\waveform_test\140831_175352_1_WAVE_OUT.txt'; % out data file
Pulse_Coord_files = 'H:\waveform_test\140831_175352_1_PULSE.csv'; % coordinate file

% Read the information of the In data file to find how many data rows/columns are
% there. This is the biggest file
ds = datastore(in_files);
dst = tall(ds);
s = size(dst);
sz = gather(s);
max_clm =sz(2)-2;  %  maximum number of columns
max_rw = sz(1)-2;  % maximum number of rows

cord_f = parfeval(@csvread,1,char(string(Pulse_Coord_files)),1); % read the coordinate file
cord = fetchOutputs(cord_f); % Bring the coordinate data into a matrix


% in_f = parfeval(@dlmread,1,char(string(in_files)),'',[0 0 max_rw 0]);
% results = fetchOutputs(in_f);

[match_ind,in_0_ind,cord_ind] = intersect(results(:,1),cord(:,1)); % find how many matching records are between Coordinate file and the In data file
[nrows, ncols] = size(in_0_ind);

% assign empty matrices to store the matching records
in = [];
out = [];
Plot_Coord = [];

% number of data sets to save. I used this one to partioned the data
% matrices which ease my processing
match_set = (fix(nrows/10000))/100;

% assiging intial and end values for the matching for loop
r2 = 100:100:nrows*100;
r1 = 1:100:(nrows*100)-100;
r3 = 1:1:match_set;
%%  
tic
for i = 1:numel(r1)
in = [];
out = [];
Plot_Coord = [];
    disp(string(i))
    for r = r1(i):r2(i)
%         disp(string(r))
        min_in = (r+(((r-1)*10000)-((r-1)*1)))-1; % minimum record ID in the list to start reading
        max_in = r*10000-1; % maximum record ID to read
        in_f = parfeval(@dlmread,1,char(string(in_files)),'',[min_in 0 max_in max_clm]); % read the data from the Incoming waveformsa (e.g.140831_175352_1_WAVE_IN_0.txt)
        in_waves = fetchOutputs(in_f); % assign the read records into a matrix 

    % extract the in_data for the plot

        In_idx = ismember(in_waves(:,1),cord(:,1)); % find the matching IDs between "In" records and "Coordinate" records
        in_waves = in_waves(In_idx,:); % modifies the in_waves only with matching "in" records 

    % out data indices
        out_id = in_waves(:,1); % IDs of matching outgoing waveforms

    % read the out data for the plot
        out_f = parfeval(@dlmread,1,char(string(out_files)),'',[min(out_id) 0 max(out_id) 60]); % read the outgoing data from 140831_175352_1_WAVE_OUT.txt
        out_waves = fetchOutputs(out_f); % save the read out waves to a matrix
        out_idx = ismember(out_waves(:,1),in_waves(:,1)); % check the maching In and Out waves (just to make sure nothing is missing
        out_waves_final = out_waves(out_idx,:); % finam matched outgoing waveforms

    % in and out data - final for the plot
        in = vertcat(in,in_waves);
        out  = vertcat(out,out_waves_final);

    % get the final coordinates. As some coordinates does not have "in" waves,
    % use final in wave indices to match both out and coodinates.

        cord_idx = ismember(cord(:,1),in_waves(:,1)); % check one final time the matching coordinates
        final_Coord = cord(cord_idx,:);
        Plot_Coord = vertcat(Plot_Coord,final_Coord); 
        
    end
 % save the matched records
filename = char(strcat('140823_180439_2_',string(r3(i)),'.mat')); % file name to save the matched records
save(filename, 'in', 'out', 'Plot_Coord','-v7.3'); % save the matched records into a .mat file
clearvars -except in_files out_files Pulse_Coord_files max_clm max_rw cord r1 r2 r3 i % remove unnecessary matrices from the workspace before the next run.
end
toc