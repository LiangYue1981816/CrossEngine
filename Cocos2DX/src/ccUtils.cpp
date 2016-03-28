/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

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
****************************************************************************/
#include "json.h"
#include "ccUtils.h"
#include "CCArray.h"
#include "CCString.h"
#include "CCMutableDictionary.h"

namespace cocos2d {

	CC_DLL unsigned long ccNextPOT(unsigned long x)
	{
		x = x - 1;
		x = x | (x >> 1);
		x = x | (x >> 2);
		x = x | (x >> 4);
		x = x | (x >> 8);
		x = x | (x >>16);
		return x + 1;
	}

	CC_DLL void ccStringReplace(std::string &str, const std::string &src, const std::string &dst)
	{
		while (true) {
			std::string::size_type pos = str.find(src);
			if (pos != std::string::npos) {
				str.replace(pos, src.size(), dst);
				continue;
			}

			break;
		}
	}

	CC_DLL CCObject* ccJsonToObject(const std::string &strJson)
	{
		CCObject *object = NULL;

		if (json_object *json = json_tokener_parse(strJson.c_str())) {
			switch (json_object_get_type(json)) {
			case json_type_object:
				{
					object = new CCDictionary<std::string, CCObject*>;
					object->autorelease();

					json_object_object_foreach(json, key, val) {
						((CCDictionary<std::string, CCObject*> *)object)->setObject(ccJsonToObject(json_object_to_json_string(val)), key);
					}
				}
				break;

			case json_type_array:
				{
					object = CCArray::array();

					for(INT index = 0; index < json_object_array_length(json); index++) {
						json_object *element = json_object_array_get_idx(json, index);
						((CCArray *)object)->addObject(ccJsonToObject(json_object_to_json_string(element)));
					}
				}
				break;

			default:
				{
					object = new CCString(json_object_get_string(json));
					object->autorelease();
				}
				break;
			}

			json_object_put(json);
		}

		return object;
	}

	static json_object* ccObjectToJson(CCObject *object)
	{
		json_object *json = NULL;

		switch (object->getObjectType()) {
		case kCCObjectTypeArray:
			{
				json = json_object_new_array();
				CCArray *arrays = (CCArray *)object;
				for (unsigned int i = 0; i < arrays->count(); i++)
				{
					json_object_array_add(json, ccObjectToJson(arrays->objectAtIndex(i)));
				}
			}
			break;

		case kCCObjectTypeDictionary:
			{
				json = json_object_new_object();
				CCDictionary<std::string, CCObject*> *dict = (CCDictionary<std::string, CCObject*> *)object;
				std::vector<std::string> keys = dict->allKeys();
				for (unsigned int i = 0; i < keys.size(); i++)
				{
					json_object_object_add(json, keys[i].c_str(), ccObjectToJson(dict->objectForKey(keys[i])));
				}
			}
			break;

		case kCCObjectTypeString:
			{
				CCString *str = (CCString *)object;
				json = json_object_new_string(str->m_sString.c_str());
			}
			break;

		case kCCObjectTypeUnKnown:
			{
				assert(false);
			}
			break;
		}

		return json;
	}

	CC_DLL const char* ccObjectToJson(CCObject *object, std::string &strJson)
	{
		strJson = "";

		if (json_object *json = ccObjectToJson(object))
		{
			strJson = json_object_get_string(json);
			json_object_put(json);
		}

		return strJson.c_str();
	}

}
