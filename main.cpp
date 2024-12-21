#ifndef F_CPU
#define F_CPU 16000000UL // Define CPU frequency as 16 MHz
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

constexpr uint16_t PULSE_MIN = 999U;  // Minimum pulse width (us)
constexpr uint16_t PULSE_MAX = 4799U; // Maximum pulse width (us)
constexpr uint16_t PULSE_MID = 2899U; // Midpoint pulse width (us)
constexpr uint16_t TOP_VALUE = 39999; // Timer TOP value for 50 Hz (20ms period)

// Function prototypes
void setupTimer();
void setServoPulse(uint16_t pulse);

int main() 
{
    // Configure Timer1 for servo control
    setupTimer();

    // Set up PB1 (OC1A) as output for the servo
    DDRB |= (1 << PB1);

    while (true) 
    {
        // Sweep from PULSE_MIN to PULSE_MAX
        for (uint16_t pulse = PULSE_MIN; pulse < PULSE_MAX; pulse += 20) 
        {
            setServoPulse(pulse);
            _delay_ms(10);
        }

        // Sweep back from PULSE_MAX to PULSE_MIN
        for (uint16_t pulse = PULSE_MAX; pulse > PULSE_MIN; pulse -= 20) 
        {
            setServoPulse(pulse);
            _delay_ms(10);
        }

        // Test specific positions with delays
        setServoPulse(PULSE_MIN);
        _delay_ms(500);

        setServoPulse(PULSE_MID);
        _delay_ms(500);

        setServoPulse(PULSE_MAX);
        _delay_ms(500);

        setServoPulse(PULSE_MID);
        _delay_ms(500);
    }

    return 0; // Not reached
}

/**
 * @brief Sets up Timer1 for Fast PWM mode with a prescaler of 8.
 *        This is configured for a 50 Hz signal on OC1A (PB1).
 */
void setupTimer() 
{
    ICR1 = TOP_VALUE;                   // Set TOP value for 50 Hz frequency
    TCCR1A |= (1 << WGM11);             // Fast PWM mode, TOP at ICR1
    TCCR1A |= (1 << COM1A1);            // Non-inverting mode for OC1A
    TCCR1B |= (1 << WGM12) | (1 << WGM13); // Fast PWM mode
    TCCR1B |= (1 << CS11);              // Prescaler set to 8
}

/**
 * @brief Sets the pulse width for the servo by updating OCR1A.
 * 
 * @param pulse The pulse width in timer counts (e.g., PULSE_MIN to PULSE_MAX).
 */
void setServoPulse(uint16_t pulse) 
{
    if (pulse < PULSE_MIN || pulse > PULSE_MAX) 
    {
        return; // Prevent invalid pulse values
    }
    OCR1A = pulse; // Update OCR1A to set the pulse width
}
