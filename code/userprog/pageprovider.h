#ifndef PAGEPROVIDER_H
#define PAGEPROVIDER_H

#include "bitmap.h"
#include "copyright.h"
#include "utility.h"
#include "translate.h"

class PageProvider
{
 private:
  BitMap* pageBitMap;
  static PageProvider* instance ;
  int numPagesProvider;	// Number of pages in the virtual
  PageProvider(int numPages); //Initialize a Page Provider, with a specific number of Physical pages.

 public:
  static PageProvider *GetInstance(int numPages);
  ~PageProvider(); //De-allocate Page Provider.

  int GetEmptyPage(); //Get the number of a free Page .
  void ReleasePage(int index); //Free a physical page in the page provider.
  unsigned int NumAvailPages(); //Return the number of the available pages in the page provider.
} ;

#endif // PAGEPROVIDER_H
