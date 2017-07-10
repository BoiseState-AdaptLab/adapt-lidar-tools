/*
 * File name: Impulse.cpp
 * Created on: 27-June-2017
 * Author: ravi
 */

#include <iostream>
#include "Impulse.hpp"


pOpener.set_file_name(fileName.c_str());
pReader = pOpener.open();
long long p;
int rc = 0;
int fi = 0;
  /*
   * Scan the files for the longest pulse, we may need to pad some records
   * with zeroes.
   */
  while(pReader->read_pulse()) {
    totalPoints++;
    if(pReader->read_waves()) {
      for (i = 0; i < pReader->waves->get_number_of_samplings(); i++) {
        sampling = pReader->waves->get_sampling(i);
        if(sampling->get_type() == PULSEWAVES_OUTGOING) {
          continue;
        }
        for (j = 0; j < sampling->get_number_of_segments(); j++ ) {
          if(maxCount < sampling->get_number_of_samples()) {
            maxCount = sampling->get_number_of_samples();
          }
        }
      }
    }
  }

  pReader->seek(0);
  int outCount = 0;
  int outFound = 0;
  while(pReader->read_pulse()) {

    /* Set GPS information */



    if(pReader->read_waves()) {
      outFound = false;
      returnCount = 0;
      
      for (i = 0; i < pReader->waves->get_number_of_samplings() && returnCount < 1; i++) {
        sampling = pReader->waves->get_sampling(i);
        if(sampling->get_type() == PULSEWAVES_OUTGOING) {
          if( outFound ) {
            printf("more that one outgoing sampling found");
            outFound++;
          }
          outFound = true;
          fprintf(wout, "%lld ", p);
          for (j = 0; j < sampling->get_number_of_segments(); j++ ) {
            sampling->set_active_segment(j);
            fi = 0;
            for (k = 0; k < maxCount; k++) {
              if(k >= sampling->get_number_of_samples()) {
                fprintf(wout, "%u ", 0);
              } 
              else {
                fprintf(wout, "%u ", sampling->get_sample(k));
              }
            }
          }
          fprintf(wout, "\n");
        } 
        else if(sampling->get_type() == PULSEWAVES_RETURNING) {
          for (j = 0; j < sampling->get_number_of_segments(); j++ ) {
            sampling->set_active_segment(j);
            /* Need new file? */
            if(j >= nwins) {
              wins = (FILE**)realloc(wins, sizeof(FILE*) * ++nwins);
              sprintf(szWaveIn, "%s_WAVE_IN_%d.txt", szBase, nwins-1);
              wins[j] = fopen(szWaveIn, "w");
            }
            fprintf(wins[j], "%lld ", p);
            for (k = 0; k < maxCount; k++) {
              if(k >= sampling->get_number_of_samples()) {
                fprintf(wins[j], "%u ", 0);
              } 
              else {
                fprintf(wins[j], "%u ", sampling->get_sample(k));
              }
            }
            fprintf(wins[j], "\n");
            returnCount++;
          }
        } 
        else {
          printf("Unknown type: %d\n", sampling->get_type());
        }
      }
    } 
    else {
      /* No data??? */
      printf("NO DATA!\n");
    }
    p++;
    if(p % 1000 == 0) {
      progress = ((double)p / (double)totalPoints) * 100.;
      printf("\r%d%% done...", (int)progress);
    }
  }

printf("\r100%% done.\n");

free(szPulseOut);
free(szWaveIn);
free(szWaveOut);
free(szScanOut);
pReader->close();
delete pReader;

fclose(pout);
fclose(wout);
for(i = 0; i < nwins; i++) {
  fclose(wins[i]);
}
free(wins);
printf("Created %d incoming wave files\n", nwins);
printf("Total points processed: %lld\n", p);
printf("Multiple outgoing samplings: %d\n", outCount);

return 0;