
// Nessecary for cmake to create a dictionary to make my libraries 
// compatible with ROOT's interpreter 

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class HistogramHelper+;
#pragma link C++ class RHtree+;

#endif