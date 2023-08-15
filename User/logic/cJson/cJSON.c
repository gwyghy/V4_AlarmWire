/*
  Copyright (c) 2009 Dave Gamble

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

/* cJSON */
/* JSON parser in C. */

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>
#include <limits.h>
#include <ctype.h>
#include "cJSON.h"
#include "Json_Analysis.h"

static const char *ep;
//Json File String is saved flag
static int stringSaveFlag = 0;
//parse json file£¬new json struct memory address and pointer
cJSON mallocJson[900] __attribute__((at(MEM_SDRAM_CJSON_MALLOC_POS)));//²âÊÔÓÃÊý×é
int mallocJsonPt = 0;
//prase string memory address
static char stringName[20] = {0};

//loop read file from fatfs, switch pointer function

static const char* v4ParaString(const char *value, int i)
{
	if((value) >= (getParaBufferAddress()))
	{
		value = setParaBufferAddress();
		return value;
	}
	return value+i;
}

//prase json return error postion pointer
const char *cJSON_GetErrorPtr(void)
{
	return ep;
}

/* Internal constructor. */
//new json struct,and clear it
static cJSON *cJSON_New_Item(void)
{
		cJSON* node = &mallocJson[mallocJsonPt];
		mallocJsonPt++;
		if (node) memset(node,0,sizeof(cJSON));
			return node;
}

uint8_t doublenumflag = 0;

/* Parse the input text to generate a number, and populate the result into item. */
static const char *parse_number(cJSON *item,const char *num)
{
	double n=0;
	uint16_t tmp = 0;
	char tmp1,tmp2;
	
	doublenumflag++;
	if (*num=='0')
	{
		n = 0;
		num = v4ParaString(num,1);			/* is zero */
	}
	if (*num>='1' && *num<='9')
	{
		do
		{
			n=(n*10.0)+(*num -'0');
			num = v4ParaString(num,1);
		}while (*num>='0' && *num<='9');	/* Number? */
	}
	tmp = n;
	if(doublenumflag == 2 || doublenumflag == 3 || doublenumflag == 9|| doublenumflag == 8)
	{
		tmp1 = (tmp>>8)&0xFF;
		tmp2 = tmp&0XFF;
		setParaDataProc(&tmp2,1);
		setParaDataProc(&tmp1,1);
	}
	else
	{
		tmp1 = tmp;
		setParaDataProc(&tmp1,1);
	}
	return num;
}

static char childSizeTmp = 0;

static const char *parse_string(cJSON *item,const char *str)
{
	const char *ptr;char *ptr2;char *out;
	if (*str!='\"') {ep=str;return 0;}	/* not a string! */
	
	memset(stringName,0,sizeof(stringName));
	out = stringName;
	
	ptr=v4ParaString(str,1);
	ptr2=out;
	while (*ptr!='\"' && *ptr)
	{
		if (*ptr!='\\') 
		{	
			*ptr2++ = *ptr;
			ptr = v4ParaString(ptr,1);
		}
	}
	*ptr2=0;
	if (*ptr=='\"')
	{
		ptr = v4ParaString(ptr,1);
		item->type=cJSON_String;
	}
	if(stringSaveFlag)
	{
		childSizeTmp++;
		out[12] = '\0';
		setParaDataProc((uint8_t *)out,13);
		item->childSize = childSizeTmp;
	}
	else
	{
		memcpy(item->name,out,12);
		childSizeTmp = 0;
	}	
	
	doublenumflag = 0;
	return ptr;
}

/* Predeclare these prototypes. */
static const char *parse_value(cJSON *item,const char *value);
static const char *parse_array(cJSON *item,const char *value);
static const char *parse_object(cJSON *item,const char *value);

/* Utility to jump whitespace and cr/lf */
static const char *skip(const char *in) 
{
	while (in && *in && (unsigned char)*in<=32)
	{
		in = v4ParaString(in,1);
	}
	return in;
}

/* Parse an object - create a new root, and populate. */
cJSON *cJSON_ParseWithOpts(const char *value,const char **return_parse_end,int require_null_terminated)
{
	const char *end=0;
	cJSON *c=cJSON_New_Item();
	ep=0;
	if (!c) return 0;       /* memory fail */

	end=parse_value(c,skip(value));
	if (!end)	{
		mallocJsonPt = 0;
		return 0;
	}	/* parse failure. ep is set. */

	return c;
}
/* Default options for cJSON_Parse */
cJSON *cJSON_Parse(const char *value) {return cJSON_ParseWithOpts(value,0,0);}

/* Parser core - when encountering text, process appropriately. */
static const char *parse_value(cJSON *item,const char *value)
{
	if (!value)						return 0;	/* Fail on null. */
	if (!strncmp(value,"null",4))	{ item->type=cJSON_NULL;  return value+4; }
	if (!strncmp(value,"false",5))	{ item->type=cJSON_False; return value+5; }
	if (!strncmp(value,"true",4))	{ item->type=cJSON_True; item->valueint=1;	return value+4; }
	if (*value=='\"')				{ return parse_string(item,value); }
	if (*value=='-' || (*value>='0' && *value<='9'))	{ return parse_number(item,value); }
	if (*value=='[')				{ return parse_array(item,value); }
	if (*value=='{')				{ return parse_object(item,value); }

	ep=value;return 0;	/* failure. */
}

/* Build an array from input text. */
static const char *parse_array(cJSON *item,const char *value)
{
	cJSON *child;
	
	stringSaveFlag = 1;
	
	if (*value!='[')	
	{
		ep=value;
		return 0;
	}	/* not an array! */
	item->type=cJSON_Array;
	value=skip(v4ParaString(value,1));
	if (*value==']')
		return v4ParaString(value,1);	/* empty array. */
	
	if(*value == '{')
	{
		item->child=child=cJSON_New_Item();
		if (!item->child) return 0;		 /* memory fail */
		value=skip(parse_value(child,skip(value)));	/* skip any spacing, get the value. */
		if (!value) return 0;
	}
	else
	{
		value=skip(parse_value(item,skip(value)));	/* skip any spacing, get the value. */
		if (!value) return 0;
	}

	while (*value==',')
	{
		value=skip(v4ParaString(value,1));
		if(*value == '{')
		{
			cJSON *new_item;
			if (!(new_item=cJSON_New_Item())) 
				return 0; 	/* memory fail */
			child->next=new_item;
			new_item->prev=child;
			child=new_item;
			value=skip(parse_value(child,skip(value)));
			if (!value) 
				return 0;	/* memory fail */
		}
		else
		{
			value=skip(parse_value(item,skip(value)));
			if (!value) 
				return 0;	/* memory fail */
		}
	}

	if (*value==']')
		return v4ParaString(value,1);	/* end of array */
	
	ep=value;
	return 0;	/* malformed. */
}


/* Build an object from the text. */
static const char *parse_object(cJSON *item,const char *value)
{
	cJSON *child;
	
	stringSaveFlag = 0;
	
	if (*value!='{')	
	{
		ep=value;
		return 0;
	}	/* not an object! */
	item->valueint = getParaAddress();
	item->type=cJSON_Object;
	value=skip(v4ParaString(value,1));
	if (*value=='}')
		return v4ParaString(value,1);	/* empty array. */
	
	item->child=child=cJSON_New_Item();
	if (!item->child) 
		return 0;
	value=skip(parse_string(child,skip(value)));
	if (*value!=':') 
	{
		ep=value;
		return 0;
	}	/* fail! */
	value=skip(parse_value(child,skip(v4ParaString(value,1))));	/* skip any spacing, get the value. */
	if (!value) 
		return 0;
	
	while (*value==',')
	{
		cJSON *new_item;
		if (!(new_item=cJSON_New_Item()))	
			return 0; /* memory fail */
		child->next=new_item;
		new_item->prev=child;
		child=new_item;
		value=skip(parse_string(child,skip(v4ParaString(value,1))));
		if (*value!=':') 
		{
			ep=value;
			return 0;
		}	/* fail! */
		value=skip(parse_value(child,skip(v4ParaString(value,1))));	/* skip any spacing, get the value. */
		if (!value) 
			return 0;
	}
	if (*value=='}')
		return v4ParaString(value,1);	/* end of array */
	ep=value;return 0;	/* malformed. */
}


