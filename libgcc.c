#include <stdint.h>
#include <stddef.h>

uint64_t __udivmoddi4(uint64_t num, uint64_t den, uint64_t *rem_p)
{
	uint64_t quot = 0, qbit = 1;

	if (den == 0) {
		// trigger exception
		return 0;
	}

	/* Left-justify denominator and count shift */
	while ((long long) den >= 0) {
		den <<= 1;
		qbit <<= 1;
	}

	while (qbit) {
		if (den <= num) {
			num -= den;
			quot += qbit;
		}
		den >>= 1;
		qbit >>= 1;
	}

	if (rem_p)
		*rem_p = num;

	return quot;
}

uint32_t __udivmodsi4(uint32_t num, uint32_t den, uint32_t *rem_p)
{
	uint32_t quot = 0, qbit = 1;

	if (den == 0) {
		// trigger exception
		return 0;
	}

	/* Left-justify denominator and count shift */
	while ((int) den >= 0) {
		den <<= 1;
		qbit <<= 1;
	}

	while (qbit) {
		if (den <= num) {
			num -= den;
			quot += qbit;
		}
		den >>= 1;
		qbit >>= 1;
	}

	if (rem_p)
		*rem_p = num;

	return quot;
}

uint32_t __udivsi3(uint32_t a, uint32_t b)
{
	return __udivmodsi4(a, b, NULL);
}

uint32_t __umodsi3(uint32_t a, uint32_t b)
{
	uint32_t v;
	(void)__udivmodsi4(a, b, &v);
	return v;
}

uint64_t __umoddi3(uint64_t a, uint64_t b)
{
	uint64_t v;
	(void)__udivmoddi4(a, b, &v);
	return v;
}

uint64_t __udivdi3(uint64_t a, uint64_t b)
{
	return __udivmoddi4(a, b, NULL);
}

int64_t __divsi3(int32_t num, int32_t den)
{
	int minus = 0;
	int32_t v;

	if (num < 0) {
		num = -num;
		minus = 1;
	}

	if (den < 0) {
		den = -den;
		minus ^= 1;
	}

	v = __udivmodsi4(num, den, NULL);
	if (minus)
		v = -v;

	return v;
}
