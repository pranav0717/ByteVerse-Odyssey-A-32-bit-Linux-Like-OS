#ifndef TESTS_H
#define TESTS_H

// all tests
int idt_test();
int div0_test();
int assertionCheck();
int page_fault_test();
int pageTesting();
int syscall_test();
int null_dereferenceTest();
int file_system_test();
int terminal_test();
int dir_open_and_read_test();
int exec_open_and_read_test();
int text_open_and_read_test();
int text_read_with_offset_test();
int exec_open_and_read_by_idx_test();

// test launcher
void launch_tests();

#endif /* TESTS_H */
