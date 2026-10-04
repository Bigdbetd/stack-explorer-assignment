/* Stack Explorer: completed recursion and callback demonstrations. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int max_depth = 0;
unsigned long long fibonacci_calls = 0;
/* Full tracing is the default; --quiet-fibonacci keeps large tests readable. */
int trace_fibonacci = 1;

/* PART 1: BASIC RECURSION */
int factorial(int n) {
    printf("Entering factorial(%d)\n", n);
    int result;
    if (n <= 1) {
        printf("Base case reached\n");
        result = 1;
    } else {
        result = n * factorial(n - 1);
    }
    printf("Returning from factorial(%d) = %d\n", n, result);
    return result;
}

/* PART 2: STACK DEPTH TRACKING */
int fibonacci(int n, int depth) {
    ++fibonacci_calls;
    if (depth > max_depth) max_depth = depth;
    if (trace_fibonacci) printf("Depth %d: fibonacci(%d)\n", depth, n);
    int result;
    if (n <= 1) result = n;
    else {
        int left = fibonacci(n - 1, depth + 1);
        int right = fibonacci(n - 2, depth + 1);
        result = left + right;
    }
    if (trace_fibonacci)
        printf("Returning depth %d: fibonacci(%d) = %d\n", depth, n, result);
    return result;
}

/* PART 3: STACK OVERFLOW DEMONSTRATION */
void infinite_recursion(int n) {
    /* The volatile local and work after the call prevent tail-call elimination. */
    volatile int frame_value = n;
    printf("Call %d\n", n);
    infinite_recursion(n + 1);
    printf("Returning from call %d\n", frame_value);
}

void safe_recursion(int n, int depth_limit) {
    printf("Call %d\n", n);
    if (n >= depth_limit) {
        printf("Stopping at max depth\n");
        return;
    }
    safe_recursion(n + 1, depth_limit);
}

/* PART 4: FUNCTION POINTERS AND CALLBACKS */
int double_value(int n) { return n * 2; }
int square_value(int n) { return n * n; }
int negate_value(int n) { return -n; }

void process_array(int *arr, int size, int (*callback)(int)) {
    if (arr == NULL || callback == NULL || size < 0) return;
    for (int i = 0; i < size; ++i) printf("%d%s", callback(arr[i]),
                                            i + 1 == size ? "\n" : " ");
}

/* PART 5: EVENT CALLBACK SYSTEM */
#define MAX_CALLBACKS 10
typedef struct EventSystem {
    void (*callbacks[MAX_CALLBACKS])(int);
    int callback_count;
} EventSystem;

void event_system_init(EventSystem *es) {
    if (es == NULL) return;
    es->callback_count = 0;
    for (int i = 0; i < MAX_CALLBACKS; ++i) es->callbacks[i] = NULL;
}

void event_system_register(EventSystem *es, void (*callback)(int)) {
    if (es == NULL || callback == NULL) {
        printf("Invalid callback registration\n");
        return;
    }
    if (es->callback_count < MAX_CALLBACKS) {
        es->callbacks[es->callback_count++] = callback;
        printf("Callback registered\n");
    } else printf("Max callbacks reached\n");
}

void event_system_trigger(EventSystem *es, int value) {
    if (es == NULL) return;
    printf("Triggering %d callbacks with value %d\n", es->callback_count, value);
    for (int i = 0; i < es->callback_count; ++i) es->callbacks[i](value);
}

void on_score_update(int score) {
    printf("  Score callback: New score is %d\n", score);
}
void on_level_change(int level) {
    printf("  Level callback: Now entering level %d\n", level);
}
void on_health_change(int health) {
    printf("  Health callback: Health is now %d\n", health);
}

int main(int argc, char **argv) {
    /* Deliberate crash is opt-in so normal runs can finish every other part. */
    if (argc == 2 && strcmp(argv[1], "--overflow") == 0) {
        setvbuf(stdout, NULL, _IONBF, 0);
        infinite_recursion(0);
        return 0;
    }
    if (argc == 2 && strcmp(argv[1], "--quiet-fibonacci") == 0)
        trace_fibonacci = 0;
    else if (argc != 1) {
        fprintf(stderr, "Usage: %s [--overflow|--quiet-fibonacci]\n", argv[0]);
        return 1;
    }
    printf("STACK EXPLORER: Function Call Mechanics\n");
    printf("\n--- Part 1: Factorial with Stack Visualization ---\n");
    int factorial_inputs[] = {5, 10};
    for (int i = 0; i < 2; ++i) {
        int n = factorial_inputs[i];
        int result = factorial(n);
        printf("factorial(%d) = %d\n", n, result);
    }
    printf("\n--- Part 2: Fibonacci with Depth Tracking ---\n");
    int fibonacci_inputs[] = {10, 20, 30};
    for (int i = 0; i < 3; ++i) {
        int n = fibonacci_inputs[i];
        max_depth = 0;
        fibonacci_calls = 0;
        int result = fibonacci(n, 1);
        printf("SUMMARY fibonacci(%d) = %d; max depth = %d; calls = %llu\n",
               n, result, max_depth, fibonacci_calls);
    }
    printf("\n--- Part 3: Stack Overflow Demo ---\n");
    printf("Run with --overflow to observe the intentional crash separately.\n");
    printf("\n--- Part 3: Safe Recursion (Fixed Version) ---\n");
    safe_recursion(0, 10);
    printf("\n--- Part 4: Function Pointers and Callbacks ---\n");
    int arr[] = {1, 2, 3, 4, 5};
    printf("Double: "); process_array(arr, 5, double_value);
    printf("Square: "); process_array(arr, 5, square_value);
    printf("Negate: "); process_array(arr, 5, negate_value);
    printf("\n--- Part 5: Event System ---\n");
    /* Independent registries distinguish score, level, and health events. */
    EventSystem score, level, health;
    event_system_init(&score);
    event_system_init(&level);
    event_system_init(&health);
    event_system_register(&score, on_score_update);
    event_system_register(&score, on_score_update);
    event_system_register(&level, on_level_change);
    event_system_register(&health, on_health_change);
    event_system_trigger(&score, 100);
    event_system_trigger(&level, 2);
    event_system_trigger(&health, 75);
    printf("Stack exploration complete!\n");
    return 0;
}
