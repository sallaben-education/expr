//
//  rdp.h
//  expr
//
//  Created by sallaben on 10/4/17.
//

#ifndef rdp_h
#define rdp_h

#include <stdlib.h>

typedef struct RDP *RDP;

extern RDP RDP_new(void);

extern void RDP_free(RDP rdp);

extern void update_symbol(RDP rdp);

#endif /* rdp_h */
