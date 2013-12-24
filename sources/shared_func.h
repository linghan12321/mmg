/* global variables */
unsigned char inxt2[3] = {1,2,0};
unsigned char iprv2[3] = {2,0,1};
unsigned char idir[4][3] = { {1,2,3}, {0,3,2}, {0,1,3}, {0,2,1} };
char idirinv[4][4] = {{-1,0,1,2},{0,-1,2,1},{0,1,-1,2},{0,2,1,-1}};
unsigned char iarf[4][3] = { {5,4,3}, {5,1,2}, {4,2,0}, {3,0,1} };
unsigned char iarfinv[4][6] = { {-1,-1,-1,2,1,0}, {-1,1,2,-1,-1,0},{2,-1,1,-1,0,-1},{1,2,-1,0,-1,-1}};
unsigned char inxt3[7] = { 1,2,3,0,1,2,3 };
unsigned char iprv3[7] = { 3,0,1,2,3,0,1 };
unsigned char iare[6][2] = { {0,1}, {0,2}, {0,3}, {1,2}, {1,3}, {2,3} };
unsigned char ifar[6][2] = { {2,3}, {1,3}, {1,2}, {0,3}, {0,2}, {0,1} };
unsigned char isar[6][2] = { {2,3}, {3,1}, {1,2}, {0,3}, {2,0}, {0,1} };
unsigned char arpt[4][3] = { {0,1,2}, {0,4,3}, {1,3,5}, {2,5,4} };

/* shared functions */

#ifdef USE_SCOTCH
/** Warn user that we overflow asked memory during scotch call */
static inline
void warnScotch(MMG5_pMesh mesh) {
  if ( mesh->info.imprim > 4 || mesh->info.ddebug ) {
    if ( mesh->info.mem >= 0 ) {
      fprintf(stdout,"  ## Warning: we will overflow the memory asked with \"-m\"");
      fprintf(stdout," option during Scotch call.\n" );
    }
  }
}
#endif

static inline
void excfun(int sigid) {
  fprintf(stdout,"\n Unexpected error:");  fflush(stdout);
  switch(sigid) {
  case SIGABRT:
    fprintf(stdout,"  *** potential lack of memory.\n");  break;
  case SIGFPE:
    fprintf(stdout,"  Floating-point exception\n"); break;
  case SIGILL:
    fprintf(stdout,"  Illegal instruction\n"); break;
  case SIGSEGV:
    fprintf(stdout,"  Segmentation fault\n");  break;
  case SIGTERM:
  case SIGINT:
    fprintf(stdout,"  Program killed\n");  break;
  }
  exit(EXIT_FAILURE);
}

/** set function pointers */
static inline
void setfunc(pMesh mesh,pSol met) {
  if ( met->size < 6 ) {
    caltet = caltet_iso;
    lenedg = lenedg_iso;
    defsiz = defsiz_iso;
    gradsiz = gradsiz_iso;
  }
  else {
    caltet = caltet_ani;
    lenedg = lenedg_ani;
    /*defsiz = defsiz_ani;
      gradsiz = gradsiz_ani;*/
  }
}


/** Common deallocations before return */
static inline
void freeCommon(pMesh mesh,pSol met
#ifdef SINGUL
             ,pSingul singul
#endif
             ){
  /* mesh */
  free(mesh->point);
  mesh->point = NULL;
  free(mesh->tetra);
  mesh->tetra = NULL;
  free(mesh->adja);
  mesh->adja = NULL;
  if ( mesh->xpoint ) {
    free(mesh->xpoint);
    mesh->xpoint = NULL;
  }
  if ( mesh->htab.geom ) {
    free(mesh->htab.geom);
    mesh->htab.geom = NULL;
  }

  if ( mesh->tria ) {
    free(mesh->tria);
    mesh->tria = NULL;
  }
  if ( mesh->xtetra ) {
    free(mesh->xtetra);
    mesh->xtetra = NULL;
  }

  /* met */
  if ( /*!mesh->info.iso &&*/ met->m ) {
    free(met->m);
    met->m = NULL;
  }

  /* mesh->info */
  if ( mesh->info.npar && mesh->info.par ) {
    free(mesh->info.par);
    mesh->info.par = NULL;
  }

#ifdef SINGUL
  /* singul */
  if ( singul->ns ) {
    free(singul->point);
    singul->point=NULL;
  }
  if ( singul->na ) {
    free(singul->edge);
    singul->edge=NULL;
  }
#endif
}
