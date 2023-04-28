/**
 * Definition for singly-linked list.
 * type ListNode struct {
 *     Val int
 *     Next *ListNode
 * }
 */
func addTwoNumbers(l1 *ListNode, l2 *ListNode) *ListNode {
	var result *ListNode = nil
	var cur *ListNode = nil
	remainder := 0
	for {
		if l1 == nil && l2 == nil && remainder == 0 {
			return result
		}
		num := remainder
		if l1 != nil {
			num += l1.Val
			l1 = l1.Next
		}
		if l2 != nil {
			num += l2.Val
			l2 = l2.Next
		}
		val := num % 10
		remainder = num / 10
		node := &ListNode{
			Val:  val,
			Next: nil,
		}
		if result == nil {
			result = node
			cur = result
		} else {
			cur.Next = node
			cur = node
		}
	}
}