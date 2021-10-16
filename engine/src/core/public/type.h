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
				static const char* GetTypeName() { return #cls; }\
			private:

#define ORB_DECLARE_TYPE_IN_NS( ns, cls )\
			public:\
				friend class core::DataCenter;\
				static const char* GetTypeName() { return ORB_STRINGIFY( ORB_CONCATENATE_STR( ns, cls ) ); }\
			private: