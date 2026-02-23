#include <iostream>
#include <algorithm>
#include <vector>

int firstMissingPositive (int* nums, int numsSize) {
    bool isExist[numsSize + 2];
    for (int i = 1; i <= numsSize + 1; i++) isExist[i] = false;
    for (int i = 0; i < numsSize; i++) {
        if (nums[i] > numsSize || nums[i] <= 0) continue;
        isExist[nums[i]] = true;
    }
    for (int i = 1; i <= numsSize + 1; i++) {
        if (isExist[i] == false) return i;
    }
    return -1;
}

// Helper function to print an array (for testing display)
void printArray(int* arr, int size) {
    std::cout << "[";
    for (int i = 0; i < size; ++i) {
        std::cout << arr[i] << (i == size - 1 ? "" : ",");
    }
    std::cout << "]";
}

void test_missing_positive(int* arr, int size, int expected) {
    std::vector<int> original_vec(arr, arr + size);
    int result = firstMissingPositive(arr, size);
    
    std::cout << "Input: "; 
    printArray(original_vec.data(), size); 
    std::cout << "\n";
    std::cout << "Output: " << result << " (Expected: " << expected << ")\n\n";
}

int main() {
    std::cout << "--- 4.2 First Missing Positive ---\n\n";

    // Example 1
    int nums1[] = {1, 2, 0};
    test_missing_positive(nums1, 3, 3);

    // Example 2
    int nums2[] = {3, 4, -1, 1};
    test_missing_positive(nums2, 4, 2);

    // Example 3
    int nums3[] = {7, 8, 9, 11, 12};
    test_missing_positive(nums3, 5, 1);

    return 0;
}