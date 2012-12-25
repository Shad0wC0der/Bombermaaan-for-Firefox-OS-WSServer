#include "JsonParser.h"

JsonParser::JsonParser(std::string source){
	this->source = source;
	errorPos = 0;
	errorDesc = 0;
	errorLine = 0;
	allocator = new block_allocator (1 << 10);

	root= json_parse((char *)this->source.c_str(), &errorPos, &errorDesc, &errorLine, allocator);

	if(root)
		value = root->first_child;
}

JsonParser::~JsonParser(){
	allocator->~block_allocator();
}

bool JsonParser::nextValue(){
	if(value)
		value=value->next_sibling;

	return value!=0;
}