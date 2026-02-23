#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>

double findMedianSortedArrays(int* nums1, int nums1Size, int* nums2, int nums2Size) {
    // To Do...

}

void test_median(int* n1, int s1, int* n2, int s2, double expected) {
    double result = findMedianSortedArrays(n1, s1, n2, s2);
    
    std::cout << "Input: nums1 = [";
    for(int i = 0; i < s1; ++i) std::cout << n1[i] << (i == s1 - 1 ? "" : ",");
    std::cout << "], nums2 = [";
    for(int i = 0; i < s2; ++i) std::cout << n2[i] << (i == s2 - 1 ? "" : ",");
    std::cout << "]\n";
    
    printf("Output: %.5f (Expected: %.5f)\n\n", result, expected);
}

int main() {
    std::cout << "--- 4.1 Median of Two Sorted Arrays ---\n\n";

    // Example 1
    int nums1_1[] = {1, 3};
    int nums2_1[] = {2};
    test_median(nums1_1, 2, nums2_1, 1, 2.00000);

    // Example 2
    int nums1_2[] = {1, 2};
    int nums2_2[] = {3, 4};
    test_median(nums1_2, 2, nums2_2, 2, 2.50000);

    return 0;
}