#include "copyright.h"

#include "system.h"
#include "pageprovider.h"


// pageprovider.cc
//       Manage Physical Pages in the memory .
//       a BitMap is Initialized in order to manage the memory's pages.
//

//
//----------------------------------------------------------------------
// PageProvider::PageProvider
//       Allocate a PageProvider .
//       Initialize a bitmap on a specific number of pages, so that every page is clear.

//      "numPages" is the number of memory pages to manage in the bitmap.
//----------------------------------------------------------------------

PageProvider* PageProvider::instance = 0;

PageProvider::PageProvider(int numPages) {
  pageBitMap = new BitMap(numPages) ;
}


//----------------------------------------------------------------------
// PageProvider::~PageProvider
//      De-allocate a PageProvider.
//----------------------------------------------------------------------

PageProvider::~PageProvider() {
  delete pageBitMap ;
}

//----------------------------------------------------------------------
// PageProvider PageProvider::GetInstance(int numPages)
//      A Singleton Pattern in order to allocate the PageProvider only once.
//----------------------------------------------------------------------

PageProvider* PageProvider::GetInstance(int numPages){
  if(instance == 0)
    instance = new PageProvider(numPages);
  return instance;
}

//----------------------------------------------------------------------
// PageProvider::GetEmptyPage(int pageSize)
//      Return the index of the empty page in the bitmap.
//      Also, it clears the page and Initializes it to 0.

//      pageSize is the size of the page to free.
//----------------------------------------------------------------------

int PageProvider::GetEmptyPage() {
  int index = pageBitMap->Find() ;

  //memset(&pageTable[index], 0, PageSize);
  memset(&(machine->mainMemory[index* PageSize]), 0, PageSize);
  return  index;
}

void PageProvider::ReleasePage(int index){
  pageBitMap->Clear(index) ;
}

unsigned int PageProvider::NumAvailPages(){
  return (unsigned int)pageBitMap->NumClear();
}
