/*
 * RequestMessage.h
 *
 *  Created on: 16 déc. 2012
 *      Author: root
 */

#ifndef JSONPARSER_H_
#define JSONPARSER_H_
#include <string>
#include "vjson/json.h"

class JsonParser{
public:
	JsonParser(const std::string&);
	virtual ~JsonParser();
	bool nextValue();
	json_value* getCurrentValue(){return value;}
private:
	json_value *root;
	json_value *value;
	std::string source;
	char *errorPos;
	char *errorDesc;
	int errorLine;
	block_allocator *allocator;
};

#endif /* JSONPARSER_H_ */
