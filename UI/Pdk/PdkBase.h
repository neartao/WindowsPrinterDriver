#ifndef PdkBase_H_ // (make sure this is only included once)
#define PdkBase_H_

#ifndef __cplusplus
#error PdkBase.h included in a non-C++ source file!
#endif

// --------------------------------------------------------------------
//              INCLUDES
// --------------------------------------------------------------------

// --------------------------------------------------------------------
//      		#DEFINES, TYPEDEF'S, and TEMPLATES
// --------------------------------------------------------------------

// --------------------------------------------------------------------
//              EXPORTED CLASSES
// --------------------------------------------------------------------


// ....................................................................
// CLASS NAME:      CPdkBase
//
// REPONSIBILITIES: This is the base class for the DrvPDK Framework.
//                  This class is meant to be a placeholder for possible
//                  future additions to the framework as needed.
//
// NOTES:
// ....................................................................
class CPdkBase
{
	public:
		// ## Constructors / Destructor
		CPdkBase () {};   // class constructor
		virtual ~CPdkBase () {}; // class destructor
		
	private:
}; 


#endif  //  PdkBase_H_
