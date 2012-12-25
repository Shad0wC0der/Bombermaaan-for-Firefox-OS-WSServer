/*
 * RequestMessage.cpp
 *
 *  Created on: 16 déc. 2012
 *      Author: root
 */

#include "RequestMessage.h"

void RequestMessage::process(void){
	con->send(parser->getCurrentValue()->first_child->string_value);

}