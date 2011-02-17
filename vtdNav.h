/* 
 * Copyright (C) 2002-2010 XimpleWare, info@ximpleware.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#ifndef VTDNAV_H
#define VTDNAV_H
#include "customTypes.h"
#include "fastIntBuffer.h"
#include "fastLongBuffer.h"
#include "contextBuffer.h"
#include "UTF8Char.h"
#include "XMLChar.h"
#include "decoder.h"
#include <math.h>
#include <stdio.h>
//#include "elementFragmentNs.h"


#define MASK_TOKEN_FULL_LEN 0x000fffff00000000LL
#define MASK_TOKEN_PRE_LEN 0x000ff80000000000LL
#define MASK_TOKEN_QN_LEN  0x000007ff00000000LL
#define MASK_TOKEN_OFFSET1  0x000000003fffffffLL
#define MASK_TOKEN_OFFSET2  0x000000007fffffffLL

#define MASK_TOKEN_TYPE  0xf000000000000000LL
#define MASK_TOKEN_DEPTH  0x0ff0000000000000LL
#define MASK_TOKEN_NS_MARK 0x00000000c0000000LL

//#define ROOT 0
//#define PARENT 1
//#define FIRST_CHILD 2
//#define LAST_CHILD 3
//#define NEXT_SIBLING 4
//#define PREV_SIBLING 5

#define R 0
#define P 1
#define FC 2
#define LC 3
#define NS 4
#define PS 5


typedef struct vTDNav{
	int rootIndex;
	int nestingLevel;
	int* context; // context object
	Boolean atTerminal; // Add this model to be compatible with XPath data model, 
						// true if attribute axis or text()
// location cache part
	int l2upper;
	int l2lower;
	int l3upper;
	int l3lower;
	int l2index;
	int l3index;
	int l1index;
    
	FastLongBuffer *vtdBuffer;
	FastLongBuffer *l1Buffer;
	FastLongBuffer *l2Buffer;
	FastIntBuffer *l3Buffer;
	UByte* XMLDoc;
    
	Long offsetMask;
	ContextBuffer *contextBuf;
	ContextBuffer *contextBuf2;// this is reserved for XPath
	
	int LN;// record txt and attrbute for XPath eval purposes

	encoding_t encoding;

	//int currentOffset;
	//int currentOffset2;

	Boolean ns;
	int* stackTemp;
	int docOffset;	 // starting offset of the XML doc wrt XMLDoc
	int docLen;  // size of XML document
	int vtdSize; // # of entries in vtdBuffer equvalent 
	             // to calling size(FastLongBuffer *flb) defined in fastLongBuffer.h
	int bufLen; // size of XMLDoc in bytes
	Boolean br; // buffer reuse flag
	Boolean master; // true if vn is obtained by calling getNav(), otherwise false
	                // useful for implementing dupliateNav() and cloneNav();
} VTDNav;


//functions
//Create VTDNav object

VTDNav *createVTDNav(int r, encoding_t enc, Boolean ns, int depth,
					 UByte *x, int xLen, FastLongBuffer *vtd, FastLongBuffer *l1,
					 FastLongBuffer *l2, FastIntBuffer *l3, int so, int len,Boolean br);

//Free VTDNav object
void freeVTDNav(VTDNav *vn);

//Return the attribute count of the element at the cursor position.
int getAttrCount(VTDNav *vn);

//Get the token index of the attribute value given an attribute name.     
int getAttrVal(VTDNav *vn, UCSChar *attrName);

//Get the token index of the attribute value of given URL and local name.
//If ns is not enabled, the lookup will return -1, indicating a no-found.
//Also namespace nodes are invisible using this method.
int getAttrValNS(VTDNav *vn, UCSChar* URL, UCSChar *localName);


//Get the depth (>=0) of the current element.
extern inline int getCurrentDepth(VTDNav *vn);

// Get the index value of the current element.
extern inline int getCurrentIndex(VTDNav *vn);
extern inline int getCurrentIndex2(VTDNav *vn);
// Get the starting offset and length of an element
// encoded in a long, upper 32 bit is length; lower 32 bit is offset
Long getElementFragment(VTDNav *vn);
Long getContentFragment(VTDNav *vn);
// Get the element fragment object corresponding to a ns 
// compensated element
struct elementFragmentNs *getElementFragmentNs(VTDNav *vn);

/**
 * Get the encoding of the XML document.
 * <pre>   0  ASCII       </pre>
 * <pre>   1  ISO-8859-1  </pre>
 * <pre>   2  UTF-8       </pre>
 * <pre>   3  UTF-16BE    </pre>
 * <pre>   4  UTF-16LE    </pre>
 */
extern inline encoding_t getEncoding(VTDNav *vn);

// Get the maximum nesting depth of the XML document (>0).
// max depth is nestingLevel -1

// max depth is nestingLevel -1
extern inline int getNestingLevel(VTDNav *vn);

// Get root index value.
extern inline int getRootIndex(VTDNav *vn);

// This function returns of the token index of the type character data or CDATA.
// Notice that it is intended to support data orient XML (not mixed-content XML).
int getText(VTDNav *vn);

//Get total number of VTD tokens for the current XML document.
extern inline int getTokenCount(VTDNav *vn);

//Get the depth value of a token (>=0)
int getTokenDepth(VTDNav *vn, int index);

//Get the token length at the given index value
//please refer to VTD spec for more details
int getTokenLength(VTDNav *vn, int index);

//Get the starting offset of the token at the given index.
extern inline int getTokenOffset(VTDNav *vn, int index);

// Get the XML document 
extern inline UByte* getXML(VTDNav *vn);

//Get the token type of the token at the given index value.
extern inline	tokenType getTokenType(VTDNav *vn, int index);

//Test whether current element has an attribute with the matching name.
Boolean hasAttr(VTDNav *vn, UCSChar *attrName);

//Test whether the current element has an attribute with 
//matching namespace URL and localname.
Boolean hasAttrNS(VTDNav *vn, UCSChar *URL, UCSChar *localName);

//This method is similar to getElementByName in DOM except it doesn't
//return the nodeset, instead it iterates over those nodes.
int iterate(VTDNav *vn, int dp, UCSChar *en, Boolean special);

//This method is similar to getElementByName in DOM except it doesn't
//return the nodeset, instead it iterates over those nodes .
//When URL is "*" it will match any namespace
//if ns is false, return false immediately
int iterateNS(VTDNav *vn, int dp, UCSChar *URL, UCSChar *ln);

// This function is called by selectElement_P in autoPilot
Boolean iterate_preceding(VTDNav *vn,UCSChar *en, int* a, Boolean special);

// This function is called by selectElementNS_P in autoPilot
Boolean iterate_precedingNS(VTDNav *vn,UCSChar *URL, UCSChar *ln, int* a);

// This function is called by selectElement_F in autoPilot
Boolean iterate_following(VTDNav *vn,UCSChar *en, Boolean special);


// This function is called by selectElementNS_F in autoPilot
Boolean iterate_followingNS(VTDNav *vn, UCSChar *URL, UCSChar *ln);


//Test if the current element matches the given name.
Boolean matchElement(VTDNav *vn, UCSChar *en);

//Test whether the current element matches the given namespace URL and localname.
//URL, when set to "*", matches any namespace (including null), when set to null, defines a "always-no-match"
//ln is the localname that, when set to *, matches any localname
Boolean matchElementNS(VTDNav *vn, UCSChar *URL, UCSChar *ln);

//Match the string against the token at the given index value. When a token
//is an attribute name or starting tag, qualified name is what gets matched against
Boolean matchRawTokenString(VTDNav *vn, int index, UCSChar *s);
//This method matches two VTD tokens of 2 VTDNavs
Boolean matchTokens(VTDNav *vn, int i1, VTDNav *vn2, int i2);

//Match the string against the token at the given index value. When a token
//is an attribute name or starting tag, qualified name is what gets matched against
Boolean matchTokenString(VTDNav *vn, int index, UCSChar *s);

//Convert a vtd token into a double.
double parseDouble(VTDNav *vn, int index);

//Convert a vtd token into a float.
float parseFloat(VTDNav *vn, int index);

//Convert a vtd token into an int
int parseInt(VTDNav *vn, int index);

//Convert a vtd token into a long
Long parseLong(VTDNav *vn, int index);

//Load the context info from ContextBuffer.
//Info saved including LC and current state of the context 
Boolean pop(VTDNav *vn);
Boolean pop2(VTDNav *vn);
//Store the context info into the ContextBuffer.
//Info saved including LC and current state of the context 
Boolean push(VTDNav *vn);
Boolean push2(VTDNav *vn);
void sampleState(VTDNav *vn, FastIntBuffer *fib);

// A generic navigation method.
// Move the current to the element according to the direction constants
// If no such element, no position change and return false (0).
/* Legal direction constants are 	<br>
	 * <pre>		ROOT            0  </pre>
	 * <pre>		PARENT          1  </pre>
	 * <pre>		FIRST_CHILD     2  </pre>
	 * <pre>		LAST_CHILD      3  </pre>
	 * <pre>		NEXT_SIBLING    4  </pre>
	 * <pre>		PREV_SIBLING    5  </pre>
	 * <br>
	 */
Boolean toElement(VTDNav *vn, navDir direction);

/**
 * A generic navigation method.
 * Move the current to the element according to the direction 
 * constants and the element name
 * If no such element, no position change and return false (0).
 * "*" matches any element
 * Legal direction constants are 	<br>
 * <pre>		ROOT            0  </pre>
 * <pre>		PARENT          1  </pre>
 * <pre>		FIRST_CHILD     2  </pre>
 * <pre>		LAST_CHILD      3  </pre>
 * <pre>		NEXT_SIBLING    4  </pre>
 * <pre>		PREV_SIBLING    5  </pre>
 * <br>
 * for ROOT and PARENT, element name will be ignored.
 */
Boolean toElement2(VTDNav *vn, navDir direction, UCSChar *en);
/*	
 * A generic navigation function with namespace support.
 * Move the current to the element according to the direction constants and the prefix and local names
 * If no such element, no position change and return false(0).
 * URL * matches any namespace, including undefined namespaces
 * a null URL means hte namespace prefix is undefined for the element
 * ln *  matches any localname
 * Legal direction constants are<br>
 * <pre>		ROOT            0  </pre>
 * <pre>		PARENT          1  </pre>
 * <pre>		FIRST_CHILD     2  </pre>
 * <pre>		LAST_CHILD      3  </pre>
 * <pre>		NEXT_SIBLING    4  </pre>
 * <pre>		PREV_SIBLING    5  </pre>
 * <br>
 * for ROOT and PARENT, element name will be ignored.
 * If not ns enabled, return false immediately with no position change.
 */
Boolean toElementNS(VTDNav *vn, navDir direction, UCSChar *URL, UCSChar *ln);

//This method normalizes a token into a string in a way that resembles DOM.
//The leading and trailing white space characters will be stripped.
//The entity and character references will be resolved
//Multiple whitespaces char will be collapsed into one.
UCSChar *toNormalizedString(VTDNav *vn, int index);

//Convert a token at the given index to a String, 
//(built-in entity and char references not resolved)
//(entities and char references not expanded).
//os and len are in bytes
UCSChar *toRawString(VTDNav *vn, int index);
UCSChar *toRawString2(VTDNav *vn, int os, int len);

//Convert a token at the given index to a String, (entities and char 
//references resolved).
// An attribute name or an element name will get the UCS2 string of qualified name 
//os and len are in bytes
UCSChar *toString(VTDNav *vn, int index);
UCSChar *toString2(VTDNav *vn, int os, int len);

/**
 * Set the value of atTerminal
 * This function only gets called in XPath eval
 * when a step calls for @* or child::text()
 */

//inline void setAtTerminal(VTDNav* vn, Boolean b);
extern inline void setAtTerminal(VTDNav* vn, Boolean b);/*{
	vn->atTerminal = b;
}*/
/**
 * Get the value of atTerminal
 * This function only gets called in XPath eval
 */
//inline Boolean getAtTerminal(VTDNav *vn);
extern inline Boolean getAtTerminal(VTDNav *vn);/*{
	return vn->atTerminal;
}*/

extern inline int swap_bytes(int i);
int lookupNS(VTDNav *vn);

/* Write VTD+XML into a FILE pointer */
//Boolean writeIndex_VTDNav(VTDNav *vn, FILE *f);

/* overwrite */
Boolean overWrite(VTDNav *vn, int index, UByte* ba, int offset, int len);

int compareTokenString(VTDNav *vn,int index, UCSChar *s);

int compareRawTokenString(VTDNav *vn, int index, UCSChar *s);

int compareTokens(VTDNav *vn, int i1, VTDNav *vn2, int i2);

/* Write VTD+XML into a FILE pointer */
Boolean writeIndex_VTDNav(VTDNav *vn, FILE *f);

/* Write VTD+XML into a file of given name */
Boolean writeIndex2_VTDNav(VTDNav *vn, char *fileName);

/* Write the VTDs and LCs into an file*/
void writeSeparateIndex_VTDNav(VTDNav *vn, char *vtdIndex);

/* pre-calculate the VTD+XML index size without generating the actual index */
Long getIndexSize2(VTDNav *vn);

/* dump XML text into a given file name */
void dumpXML(VTDNav *vn, char *fileName);

/* dump XML text into a given file descriptor */
void dumpXML2(VTDNav *vn, FILE *f);

/*Get the string length as if the token is converted into a normalized UCS string */
int getNormalizedStringLength(VTDNav *vn, int index);
/*Get the string length as if the token is converted into a UCS string (entity resolved) */
int getStringLength(VTDNav *vn, int index);
/*Get the string length as if the token is converted into a UCS string (entity not resolved) */
int getRawStringLength(VTDNav *vn, int index);
/* Get the offset value right after head (e.g. <a b='b' c='c'> ) */
int getOffsetAfterHead(VTDNav *vn);

/* Test the start of token content at index i matches the content 
of s, notice that this is to save the string allocation cost of
using String's built-in startsWidth */
Boolean startsWith(VTDNav *vn, int index, UCSChar *s);

/*Test the end of token content at index i matches the content 
of s, notice that this is to save the string allocation cost of
using String's built-in endsWidth */

Boolean endsWith(VTDNav *vn, int index, UCSChar *s);

/*Test whether a given token contains s. notie that this function
directly operates on the byte content of the token to avoid string creation */

Boolean contains(VTDNav *vn, int index, UCSChar *s);

/* Convert a token at the given index to a String and any lower case
   character will be converted to upper case, (entities and char
   references resolved).*/
UCSChar *toStringUpperCase(VTDNav *vn, int index);

/* Convert a token at the given index to a String and any upper case
   character will be converted to lower case, (entities and char
   references resolved).*/
UCSChar *toStringLowerCase(VTDNav *vn, int index);

/* Convert a token at the given index to a String and any lower case
   character will be converted to upper case, (entities and char
   references resolved for character data and attr val).*/
UCSChar *toRawStringUpperCase(VTDNav *vn, int index);

/* Convert a token at the given index to a String and any upper case
   character will be converted to lower case, (entities and char
   references resolved for character data and attr val).*/
UCSChar *toRawStringLowerCase(VTDNav *vn, int index);

/* DupliateNav duplicates an instance of VTDNav but doesn't retain the original node position*/
VTDNav *duplicateNav(VTDNav *vn);
/* ClineNav duplicates an instance of VTDNav, also copies node position over */
VTDNav *cloneNav(VTDNav *vn);

/* This method takes a vtd index, and recover its correspondin
 * node position, the index can only be of node type element,
 * document, attribute name, attribute value or character data,
 * or CDATA  */
void recoverNode(VTDNav *vn, int index);
/**
	 * Match the string against the token at the given index value. The token will be
     * interpreted as if it is normalized (i.e. all white space char (\r\n\a ) is replaced
     * by a white space, char entities and entity references will be replaced by their correspondin
     * char see xml 1.0 spec interpretation of attribute value normalization) */

Boolean matchNormalizedTokenString2(VTDNav *vn,int index, UCSChar *s);

/**
	 * Return the byte offset and length of up to i sibling fragments. If 
	 * there is a i+1 sibling element, the cursor element would 
	 * move to it; otherwise, there is no cursor movement. If the cursor isn't 
	 * positioned at an element (due to XPath evaluation), then -1 will be 
	 * returned
	 * @param i number of silbing elements including the cursor element
	 * @return a long encoding byte offset (bit 31 to bit 0), length (bit 62 
	 * to bit 32) of those fragments 
	 * @throws NavException
	 */

Long getSiblingElementFragments(VTDNav *vn,int i);

/**
	 * Match the string against the token at the given index value. The token will be
     * interpreted as if it is normalized (i.e. all white space char (\r\n\a ) is replaced
     * by a white space, char entities and entity references will be replaced by their correspondin
     * char see xml 1.0 spec interpretation of attribute value normalization) 
	 */
Boolean matchNormalizedTokenString2(VTDNav *vn, int index, UCSChar *s);

/**
	 * (New since version 2.9)
	 * Shallow Normalization follows the rules below to normalize a token into
	 * a string
	 * *#xD#xA gets converted to #xA
	 * *For a character reference, append the referenced character to the normalized value.
	 * *For an entity reference, recursively apply step 3 of this algorithm to the replacement text of the entity.
	 * *For a white space character (#x20, #xD, #xA, #x9), append a space character (#x20) to the normalized value.
	 * *For another character, append the character to the normalized value.*/
UCSChar* toNormalizedString2(VTDNav *vn, int index);


/** new since 2.9
	Return the prefix of a token as a string if the token 
    is of the type of starting tag, attribute name, if the 
    the prefix doesn't exist, a null string is returned;
    otherwise a null string is returned
*/
UCSChar *getPrefixString(VTDNav *vn, int index);
#endif
