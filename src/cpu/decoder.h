#ifndef ENDSDECODERH
#define ENDSDECODERH

#include "../types.h"

typedef struct search_node
{
	u32 id;	
	void (*ifunction)(void);

	struct search_node *parent;
	struct search_node **leafs;
};


#endif //ENDSDECODERH

