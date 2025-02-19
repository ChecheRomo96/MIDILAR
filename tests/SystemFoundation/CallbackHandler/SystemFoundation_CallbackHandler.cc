#include <gtest/gtest.h>
#include <SystemFoundation/Clock/Clock.h>
#include <chrono>

using namespace std::chrono;
using namespace MIDILAR::SystemFoundation;

namespace MIDILAR::SystemFoundation{
    
    namespace{
        
        uint8_t id; // Global variable to hold the ID set by the functions
        uint8_t counter = 0; // Global counter variable

        // Function that sets the global id to 1
        void fn_a(){
            id = 1;
        }

        // Function that sets the global id to 2 and returns the current counter value
        uint8_t fn_b(){
            id = 2;
            return counter++;
        } 

        // Function that sets the global id to 3 and updates the counter to a specified value
        void fn_c(uint8_t x){
            id = 3;
            counter = x;
        }
        
        // Test fixture class for testing CallbackHandler
        class CallbackHandlerTest : public testing::Test {
        private:

        protected:
        // Constructor for the test fixture
        CallbackHandlerTest() {
        }

        // Destructor for clean-up after tests
        ~CallbackHandlerTest() override {
            
        }

        // Sets up the test environment before each test
        void SetUp() override {
            id = 0; // Reset the id
            counter = 0; // Reset the counter
        }

        // Clean-up code can be added here (no-op in this case)
        void TearDown() override {
        }

        // Class members can be declared here for use across all tests

        };
    }
}

// Test case for default constructor and basic functionality of CallbackHandler
TEST(CallbackHandlerTest, SpecializationTesting) {
    
    // Create instances of CallbackHandler with various parameter types
    CallbackHandler<void, void>   myCallback1;
    CallbackHandler<uint8_t,void> myCallback2;
    CallbackHandler<void,uint8_t> myCallback3;

    // Ensure initial status of all callbacks is false (not bound)
    ASSERT_EQ(myCallback1.status(),false);
    ASSERT_EQ(myCallback2.status(),false);
    ASSERT_EQ(myCallback3.status(),false);

    // Bind the functions to the callback handlers
    myCallback1.bind(fn_a);
    myCallback2.bind(fn_b);
    myCallback3.bind(fn_c);

    // Verify that the callbacks are now bound (status true)
    ASSERT_EQ(myCallback1.status(),true);
    ASSERT_EQ(myCallback2.status(),true);
    ASSERT_EQ(myCallback3.status(),true);

    // Invoke the first callback and check expected outcomes
    myCallback1.invoke();
    ASSERT_EQ(id,1); // Check if 'id' was set correctly in fn_a
    EXPECT_EQ(counter,0); // Ensure counter remains unchanged

    // Invoke the second callback and check expected outcomes
    auto value = myCallback2.invoke();
    ASSERT_EQ(id,2); // Check if 'id' was set correctly in fn_b
    ASSERT_EQ(value,0); // Value returned by fn_b should be 0 as counter was 0
    ASSERT_EQ(counter,1); // Counter should increment now

    // Invoke the third callback with a value and check expected outcomes
    myCallback3.invoke(0xFF);
    ASSERT_EQ(id,3); // Check if 'id' was set correctly in fn_c
    ASSERT_EQ(counter,0xFF); // Counter should now equal the value provided

    // Unbind all callbacks and check their status
    myCallback1.unbind();
    myCallback2.unbind();
    myCallback3.unbind();

    ASSERT_EQ(myCallback1.status(),false); // Verify callback1 is now unbound
    ASSERT_EQ(myCallback2.status(),false); // Verify callback2 is now unbound
    ASSERT_EQ(myCallback3.status(),false); // Verify callback3 is now unbound
    
    // Invoke the first callback again after unbinding and check outcomes
    myCallback1.invoke();
    ASSERT_EQ(id,3); // Should still reflect the last 'id' value from fn_c
    EXPECT_EQ(counter,0xFF); // Counter value should remain unchanged

    // Invoke the second callback again after unbinding and check outcomes
    value = myCallback2.invoke();
    ASSERT_EQ(id,3); // Should still reflect the last 'id' value from fn_c
    ASSERT_EQ(value,uint8_t()); // Value returned should be uint8_t(), since it is unbound 
    ASSERT_EQ(counter,0xFF); // Counter should remain unchanged

    // Invoke the third callback again after unbinding and check outcomes
    myCallback3.invoke(0xFF);
    ASSERT_EQ(id,3); // Should still reflect the last 'id' value from fn_c
    ASSERT_EQ(counter,0xFF); // Counter value should remain unchanged
}