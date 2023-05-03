func findDifference(nums1 []int, nums2 []int) [][]int {
    diff1 := make(map[int]bool)
    diff2 := make(map[int]bool)
    for _, num := range nums1 {
        diff1[num] = true
    }
    for _, num := range nums2 {
        diff2[num] = true
        if _, ok := diff1[num]; ok {
            delete(diff1, num)
        }
    }
    for _, num := range nums1 {
        if _, ok := diff2[num]; ok {
            delete(diff2, num)
        }
    }
    res := make([][]int, 2)
    res[0] = make([]int, 0, len(diff1))
    res[1] = make([]int, 0, len(diff2))
    for k, v := range diff1 {
        if v {
            res[0] = append(res[0], k)
        }
    }
    for k, v := range diff2 {
        if v {
            res[1] = append(res[1], k)
        }
    }
    return res
}