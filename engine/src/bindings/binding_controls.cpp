#include <bindings/binding_controls.h>


namespace engine::binding
{
    void merge_value_to_snapshot( InputSnapshot& snapshot, const input_value_variant_t value )
    {
        // TODO: actually combine the values
        snapshot.value = value;
    }

}
