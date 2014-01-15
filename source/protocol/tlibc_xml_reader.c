#include "protocol/tlibc_xml_reader.h"
#include "lib/tlibc_code.h"
#include "protocol/tlibc_abstract_reader.h"
#include "lib/tlibc_error_code.h"
#include "tlibc_xml_reader_l.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>


TLIBC_ERROR_CODE xml_reader_init(TLIBC_XML_READER *self, const char *file_name)
{
	FILE* fin;
	char c;
	YYCTYPE* yy_start;
	tuint32 i = 0;
	tuint32 len = 0;
	SCANNER_CONTEXT *scanner = NULL;

	strncpy(self->yylloc.file_name, file_name, TLIBC_MAX_FILE_PATH_LENGTH);
	self->yylloc.file_name[TLIBC_MAX_FILE_PATH_LENGTH - 1] = 0;


	self->buff_curr = self->buff;
	self->buff_limit = self->buff + MAX_LEX_BUFF_SIZE;

	fin = fopen(file_name, "rb");
	while((c = (char)fgetc(fin)) != EOF)
	{
		if(self->buff_curr == self->buff_limit)
		{
			goto ERROR_RET;
		}
		*self->buff_curr = c;
		++(self->buff_curr);
	}

	scanner = &self->scanner_context;
	yy_start = self->buff;
	scanner->yy_start = yy_start;
	scanner->yy_limit = self->buff_curr;
	scanner->yy_state = yycINITIAL;
	scanner->yy_marker = scanner->yy_start;
	scanner->yy_last = scanner->yy_start;
	scanner->yy_cursor = scanner->yy_start;	
	scanner->yylineno = 1;
	scanner->yycolumn = 1;

	self->level = 0;









	tlibc_abstract_reader_init(&self->super);


	self->super.read_enum_name = xml_read_enum_name;
	self->super.read_struct_begin = xml_read_struct_begin;
	self->super.read_struct_end = xml_read_struct_end;

	self->super.read_vector_begin = xml_read_vector_begin;
	self->super.read_vector_end = xml_read_vector_end;
	self->super.read_field_begin = xml_read_field_begin;
	self->super.read_field_end = xml_read_field_end;
	self->super.read_vector_item_begin = xml_read_vector_item_begin;
	self->super.read_vector_item_end= xml_read_vector_item_end;

	self->super.read_tint8 = xml_read_tint8;
	self->super.read_tint16 = xml_read_tint16;
	self->super.read_tint32 = xml_read_tint32;
	self->super.read_tint64 = xml_read_tint64;

	self->super.read_tuint8 = xml_read_tuint8;
	self->super.read_tuint16 = xml_read_tuint16;
	self->super.read_tuint32 = xml_read_tuint32;
	self->super.read_tuint64 = xml_read_tuint64;

	self->super.read_tdouble = xml_read_tdouble;
	self->super.read_string = xml_read_tstring;
	self->super.read_tchar = xml_read_tchar;

	return E_TLIBC_NOERROR;
ERROR_RET:
	return E_TLIBC_ERROR;
}

tint32 xml_read_enum_name(TLIBC_ABSTRACT_READER *self, tchar *enum_name, tuint32 enum_name_length)
{
	xml_read_tstring(self, enum_name, enum_name_length);
	return E_TLIBC_NOERROR;
}

tint32 xml_read_struct_begin(TLIBC_ABSTRACT_READER *super, const char *struct_name)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);	
	
	if(self->level == 0)
	{
		XML_TOKEN_VALUE token_value;
		xml_scan(self, &token_value);
	}
	
	++self->level;
	
	return E_TLIBC_NOERROR;
}

tint32 xml_read_struct_end(TLIBC_ABSTRACT_READER *super, const char *struct_name)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	
	if(self->level == 0)
	{
		XML_TOKEN_VALUE token_value;
		xml_scan(self, &token_value);
	}
	
	--self->level;
	return E_TLIBC_NOERROR;
}

tint32 xml_read_vector_begin(TLIBC_ABSTRACT_READER *super)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);	

	return E_TLIBC_NOERROR;
}

tint32 xml_read_vector_end(TLIBC_ABSTRACT_READER *super)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);


	return E_TLIBC_NOERROR;
}

tint32 xml_read_field_begin(TLIBC_ABSTRACT_READER *super, const char *var_name)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	XML_TOKEN_VALUE token_value;
	xml_scan(self, &token_value);

	return E_TLIBC_NOERROR;
}

tint32 xml_read_field_end(TLIBC_ABSTRACT_READER *super, const char *var_name)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);	
	XML_TOKEN_VALUE token_value;
	xml_scan(self, &token_value);
	return E_TLIBC_NOERROR;
}

TLIBC_API tint32 xml_read_vector_item_begin(TLIBC_ABSTRACT_READER *super, tuint32 index)
{	
	char str[1024];
	snprintf(str, 1024, "[%d]", index);
	return xml_read_field_begin(super, str);
}

TLIBC_API tint32 xml_read_vector_item_end(TLIBC_ABSTRACT_READER *super, tuint32 index)
{
	char str[1024];
	snprintf(str, 1024, "[%d]", index);
	return xml_read_field_end(super, str);
}

static void get_content(TLIBC_XML_READER *self)
{

}

tint32 xml_read_tdouble(TLIBC_ABSTRACT_READER *super, double *val)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	
	*self->content_end = 0;
	sscanf(self->content_begin, "%lf", val);
	*self->content_end = '<';

	return E_TLIBC_NOERROR;
}

tint32 xml_read_tint8(TLIBC_ABSTRACT_READER *super, tint8 *val)
{
	tint64 i64;
	tint32 ret = xml_read_tint64(super, &i64);
	*val = (tint8)i64;
	return ret;
}

tint32 xml_read_tint16(TLIBC_ABSTRACT_READER *super, tint16 *val)
{
	tint64 i64;
	tint32 ret = xml_read_tint64(super, &i64);
	*val = (tint16)i64;
	return ret;
}

tint32 xml_read_tint32(TLIBC_ABSTRACT_READER *super, tint32 *val)
{
	tint64 i64;
	tint32 ret = xml_read_tint64(super, &i64);
	*val = (tint32)i64;
	return ret;
}

tint32 xml_read_tint64(TLIBC_ABSTRACT_READER *super, tint64 *val)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	*self->content_end = 0;
	sscanf(self->content_begin, "%lld", val);
	*self->content_end = '<';
	return E_TLIBC_NOERROR;
}

tint32 xml_read_tuint8(TLIBC_ABSTRACT_READER *super, tuint8 *val)
{
	tuint64 ui64;
	tint32 ret = xml_read_tuint64(super, &ui64);
	*val = (tuint8)ui64;
	return ret;
}

tint32 xml_read_tuint32(TLIBC_ABSTRACT_READER *super, tuint32 *val)
{
	tuint64 ui64;
	tint32 ret = xml_read_tuint64(super, &ui64);
	*val = (tuint32)ui64;
	return ret;
}

tint32 xml_read_tuint16(TLIBC_ABSTRACT_READER *super, tuint16 *val)
{
	tuint64 ui64;
	tint32 ret = xml_read_tuint64(super, &ui64);
	*val = (tuint16)ui64;
	return ret;
}

tint32 xml_read_tuint64(TLIBC_ABSTRACT_READER *super, tuint64 *val)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	*self->content_end = 0;
	sscanf(self->content_begin, "%llu", val);
	*self->content_end = '<';
	return E_TLIBC_NOERROR;
}


static void read_char(TLIBC_XML_READER* self, tchar *ch)
{
	char c = *self->content_begin++;
	
	if(c == '&')
	{		
		char c2 = *self->content_begin++;
		if(c2 == 'l')
		{
			//&lt
			*ch = '<';
		}
		else if(c2 == 'g')
		{
			//&gt
			*ch = '>';
		}
		else
		{
			char c3 = *self->content_begin++;
			if(c3 == 'm')
			{
				//&amp
				*ch = '&';
			}
			else if(c3 == 'p')
			{
				//&apos
				*ch = '\'';
			}
			else if(c3 == 'u')
			{
				//&auot
				*ch = '\"';
			}
		}
	}
	else
	{
		*ch = c;
	}
}

tint32 xml_read_tchar(TLIBC_ABSTRACT_READER *super, char *val)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);

	read_char(self, val);

	return E_TLIBC_NOERROR;
}

tint32 xml_read_tstring(TLIBC_ABSTRACT_READER *super, tchar *str, tuint32 str_len)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	tuint32 len = 0;

	while(self->content_begin < self->content_end)
	{
		char c;
		read_char(self, &c);
		str[len++] = c;
	}
	str[len] = 0;

	return E_TLIBC_NOERROR;
}



void xml_locate(TLIBC_XML_READER *self)
{
	const char *i;
	SCANNER_CONTEXT *sp = &self->scanner_context;
	for(i = sp->yy_last; i < sp->yy_cursor;++i)
	{
		if(*i == '\n')
		{
			++(sp->yylineno);
			sp->yycolumn = 1;
		}		
		else if(*i == '\r')
		{
			sp->yycolumn = 1;			
		}
		else
		{
			++(sp->yycolumn);
		}
	}
	sp->yy_last = sp->yy_cursor;
}

int xml_scan(TLIBC_XML_READER *self, XML_TOKEN_VALUE *token_value)
{
	YYCTYPE *iter;
	int token = tok_end;

	token = tlibc_xml_scan(self, &self->yylloc);
	if(token == tok_end)
	{
		goto done;
	}
	self->yylloc.last_line = self->scanner_context.yylineno;
	self->yylloc.last_column = self->scanner_context.yycolumn;

	switch(token)
	{
	case tok_tag_begin:
		{
			if(yyleng - 2 >= TLIBC_MAX_IDENTIFIER_LENGTH)
			{
				goto ERROR_RET;
			}
			memcpy(token_value->tag_start, yytext + 1, yyleng - 2);
			token_value->tag_start[yyleng - 2] = 0;

			//��¼content
			self->content_begin = self->scanner_context.yy_cursor;
			for(iter = self->content_begin; iter < self->scanner_context.yy_limit; ++iter)
			{
				if(*iter == '<')
				{
					self->content_end = iter;
					break;
				}
			}
			if(*iter != '<')
			{
				goto ERROR_RET;
			}
		}
	case tok_tag_end:
		{
			if(yyleng - 3 >= TLIBC_MAX_IDENTIFIER_LENGTH)
			{
				goto ERROR_RET;
			}
			memcpy(token_value->tag_end, yytext + 2, yyleng - 3);
			token_value->tag_end[yyleng - 3] = 0;
			break;
		}
	}
done:
	return token;

ERROR_RET:
	return tok_error;
}