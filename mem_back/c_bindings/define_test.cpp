#include "../static/registry.h"
#include "./capi_define_helper.h"

IMB_CAPI_BEGIN

IMB_CAPI_TYPE_HEADERS(float);
IMB_CAPI_TYPE_HEADERS(int32_t);

IMB_CAPI_TYPE_IMPL(float);
IMB_CAPI_TYPE_IMPL(int32_t);

IMB_CAPI_END



int main(){
	imb::registry<float> reg;

	registry_float reg_float;
	int rc = registry_create_float(&reg_float);
	registry_destroy_float(&reg_float);

	return 0;
}
