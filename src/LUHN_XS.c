

#include <stdio.h>
#include <string.h>
int   _al_vc[256]={0};
char  _al_lasterr[1024];

char* _al_get_lasterr() {
    return _al_lasterr;
}

int _al_init_vc(SV* hash_ref) {
  HV* hash;
  HE* hash_entry;
  int num_keys, i;
  SV* sv_key;
  SV* sv_val;
  for (i=0;i<256;++i) {
      _al_vc[i]=-1;
  }
  hash = (HV*)SvRV(hash_ref);
  num_keys = hv_iterinit(hash);
  for (i = 0; i < num_keys; i++) {
    hash_entry = hv_iternext(hash);
    sv_key = hv_iterkeysv(hash_entry);
    sv_val = hv_iterval(hash, hash_entry);
    _al_vc[(SvPV(sv_key,PL_na))[0]]=atoi(SvPV(sv_val,PL_na));
  }
  return 1;
}

int _al_check_digit(AV* list) {
    int i,num,value;
    int sum = 0;
    int twoup = 1;
    int len=av_len(list);
    for(i=0;i<=len;++i) {
        SV** element=av_fetch(list,i,0);
        char *str=SvPV(*element,PL_na);
        value=(int)str[0];
        num=_al_vc[value];
        if (num == -1)  { 
          snprintf(_al_lasterr,1024,"Invalid character '%c', in check_digit calculation",str[0]);
          return -1;
        } 
        if (!(twoup = !twoup)) {
            num *= 2;
        } 
        while (num) {
           sum += num % 10;
           num=(int)(num/10);
        }
    }
    return (10-(sum %10)) % 10;
}


