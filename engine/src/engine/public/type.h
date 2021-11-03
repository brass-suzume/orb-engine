#pragma once

#define ORB_SUPER_CLASS( cls )\
			private:\
				using Super = cls;

#define ORB_DECLARE_TYPE( cls )\
			public:\
				virtual size_t GetClassID()\
				{\
					static size_t s_cachedTypeID = typeid( cls ).hash_code();\
					return s_cachedTypeID;\
				}\
				friend class DataCenter;\
				static const char* GetTypeName();\
			private: