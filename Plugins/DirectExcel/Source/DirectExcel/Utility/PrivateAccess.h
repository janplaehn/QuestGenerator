// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#pragma  once
#include "CoreMinimal.h"



namespace Insider
{
	namespace PrivateAccess
	{
		//used to steal class private member
		template<typename Tag, typename Tag::type M>
		struct PrivateMemberStealer
		{
			friend typename Tag::type GetPrivate(Tag) { return M; }
		};
	}

}


#define INSIDER_STEAL_PRIVATE_MEMBER(className,memberType,memberName) \
	namespace Insider{namespace PrivateAccess{\
		struct className##_##memberName \
		{\
			typedef memberType className::*type;\
			friend type GetPrivate(className##_##memberName);\
		};\
		template struct PrivateMemberStealer<className##_##memberName, &className::memberName>;\
	}}

#define INSIDER_REF_PRIVATE_MEMBER(obj,className,memberName) obj->*GetPrivate(::Insider::PrivateAccess::className##_##memberName())
#define INSIDER_VAR_PRIVATE_MEMBER(var,obj,className,memberName) auto& var=obj->*(GetPrivate(::Insider::PrivateAccess::className##_##memberName()));


#define INSIDER_STEAL_PRIVATE_FUNCTION(className,memberName,returnType,...) \
	namespace Insider{namespace PrivateAccess{\
		struct className##_##memberName \
		{\
			typedef returnType (className::*type)(__VA_ARGS__);\
			friend type GetPrivate(className##_##memberName);\
		};\
		template struct PrivateMemberStealer<className##_##memberName, &className::memberName>;\
	}}



#define INSIDER_REF_PRIVATE_FUNCTION(obj,className,memberName) GetPrivate(::Insider::PrivateAccess::className##_##memberName())
#define INSIDER_PRIVATE_FUNCTION_CALL(obj,className,memberName,...) {auto func=GetPrivate(::Insider::PrivateAccess::className##_##memberName());(obj->*func)(__VA_ARGS__);}

