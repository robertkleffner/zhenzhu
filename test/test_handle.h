
BEGIN_TEST("Simple handle with no handlers besides the after closure.");

// main =
//   2 handle {
//     2 4 add-double
//   } with {
//     after => 1 add-double
//   }
//   sub-double

CONSTANT(NUMBER_VAL(2));
CONSTANT(NUMBER_VAL(4));
CONSTANT(NUMBER_VAL(1));

WRITE_INT_INST(CALL, 10, 1); // 5
WRITE_INT_INST(TAILCALL, 41, 2); //10

// main
WRITE_INST(CONSTANT, 3);
WRITE_BYTE(0, 3);

WRITE_INT_INST(CLOSURE, 37, 4); // 17
WRITE_BYTE(0, 4); // 18
WRITE_SHORT(0, 4); // 20

WRITE_INST(HANDLE, 5); // 21
WRITE_SHORT(6, 5); // 23
WRITE_INT(0, 5); // 27
WRITE_BYTE(0, 5); // 28
WRITE_BYTE(0, 5); // 29

WRITE_INST(CONSTANT, 6); //30
WRITE_BYTE(0, 6); // 31
WRITE_INST(CONSTANT, 7); // 32
WRITE_BYTE(1, 7); // 33
WRITE_INST(ADD, 8); // 34

WRITE_INST(COMPLETE, 9);
WRITE_INST(SUBTRACT, 10);
WRITE_INST(RETURN, 11); // 37

// ret1
WRITE_INST(CONSTANT, 12);
WRITE_BYTE(2, 12);
WRITE_INST(ADD, 13);
WRITE_INST(RETURN, 14); // 41

// end
WRITE_INST(ABORT, 15);
WRITE_BYTE(0, 15);

VERIFY_FRAMES(0);
VERIFY_STACK(1);
VERIFY_NUMBER(5.0);

END_TEST();




BEGIN_TEST("Handle with one single-shot resume handler and a closure with a parameter.");

// main =
//   handle {
//     3 4 raise! 6 div
//   } with {
//     raise! e => 2 e sub resume
//     after => 1 add
//   }

CONSTANT(NUMBER_VAL(3));
CONSTANT(NUMBER_VAL(4));
CONSTANT(NUMBER_VAL(6));
CONSTANT(NUMBER_VAL(2));
CONSTANT(NUMBER_VAL(1));

WRITE_INT_INST(CALL, 10, 1); // 5
WRITE_INT_INST(TAILCALL, 64, 2); //10

// main
WRITE_INT_INST(CLOSURE, 60, 4);
WRITE_BYTE(0, 4);
WRITE_SHORT(0, 4);

WRITE_INT_INST(CLOSURE, 51, 5);
WRITE_BYTE(1, 5);
WRITE_SHORT(0, 5);
WRITE_INST(CLOSURE_ONCE_TAIL, 5);

WRITE_INST(HANDLE, 6); // 27
WRITE_SHORT(14, 6); // 29
WRITE_INT(0, 6); // 33
WRITE_BYTE(0, 6); // 34
WRITE_BYTE(1, 6); // 35

WRITE_INST(CONSTANT, 7); // 36
WRITE_BYTE(0, 7);
WRITE_INST(CONSTANT, 7); // 38
WRITE_BYTE(1, 7);

WRITE_INST(ESCAPE, 7); // 40
WRITE_INT(0, 7); // 44
WRITE_BYTE(0, 7); // 45

WRITE_INST(CONSTANT, 7); // 46
WRITE_BYTE(2, 7);
WRITE_INST(DIVIDE, 7); // 48

WRITE_INST(COMPLETE, 8);
WRITE_INST(RETURN, 8); // 50

// raise1
WRITE_INST(CONSTANT, 9);
WRITE_BYTE(3, 9); // 52

WRITE_INST(FIND, 9);
WRITE_SHORT(0, 9);
WRITE_SHORT(0, 9);
WRITE_INST(SUBTRACT, 9);
WRITE_INST(RETURN, 10);

//WRITE_INST(FIND, 10);
//WRITE_SHORT(0, 10);
//WRITE_SHORT(0, 10);
//WRITE_INST(TAILCALL_CONTINUATION, 10);

// ret1
WRITE_INST(CONSTANT, 12); // 65
WRITE_BYTE(4, 12);
WRITE_INST(ADD, 13);
WRITE_INST(RETURN, 14); // 68

// end
WRITE_INST(ABORT, 15); // 69
WRITE_BYTE(0, 15);

VERIFY_FRAMES(0);
VERIFY_STACK(2);
VERIFY_NUMBER(4.0);

END_TEST();




BEGIN_TEST("Handler with parameters and two actions.");

// main =
//   let counter = get! 1 add-double put! zap get! 1 add-double put!
//   2
//   handle s { counter }
//   with {
//     get! => s s resume
//     put! n => [] n resume
//     after => [] swap cons s cons
//   }

CONSTANT(NUMBER_VAL(2));
CONSTANT(NUMBER_VAL(1));

WRITE_INT_INST(CALL, 10, 1); // 5
WRITE_INT_INST(TAILCALL, 122, 2); //10

// main
WRITE_INST(CONSTANT, 3);
WRITE_BYTE(0, 3); // 12

WRITE_INT_INST(CLOSURE, 112, 3); // 17
WRITE_BYTE(0, 3);
WRITE_SHORT(0, 3); // 20

WRITE_INT_INST(CLOSURE, 100, 3); // 25
WRITE_BYTE(1, 3);
WRITE_SHORT(0, 3); // 28

WRITE_INT_INST(CLOSURE, 84, 3); // 33
WRITE_BYTE(0, 3);
WRITE_SHORT(0, 3); // 36

WRITE_INST(HANDLE, 4); // 37
WRITE_SHORT(6, 4); // 39
WRITE_INT(0, 4); // 43
WRITE_BYTE(1, 4); // 44
WRITE_BYTE(2, 4); // 45

WRITE_INT_INST(CALL, 52, 4); // 50

WRITE_INST(COMPLETE, 5);
WRITE_INST(RETURN, 5); // 52

// counter
WRITE_INST(ESCAPE, 6);
WRITE_INT(0, 6); // 57
WRITE_BYTE(0, 6); // 58

WRITE_INST(CONSTANT, 6);
WRITE_BYTE(1, 6);
WRITE_INST(ADD, 6); // 61

WRITE_INST(ESCAPE, 6);
WRITE_INT(0, 6); // 66
WRITE_BYTE(1, 6); // 67

WRITE_INST(ZAP, 6);
WRITE_INST(ESCAPE, 6); // 69
WRITE_INT(0, 6); // 73
WRITE_BYTE(0, 6); // 74

WRITE_INST(CONSTANT, 6);
WRITE_BYTE(1, 6);
WRITE_INST(ADD, 6); // 77

WRITE_INST(ESCAPE, 6);
WRITE_INT(0, 6); // 82
WRITE_BYTE(1, 6); // 83

WRITE_INST(RETURN, 6);

// get1
WRITE_INST(FIND, 7);
WRITE_SHORT(0, 7); // 87
WRITE_SHORT(1, 7); // 89
WRITE_INST(FIND, 7);
WRITE_SHORT(0, 7); // 92
WRITE_SHORT(1, 7); // 94
WRITE_INST(FIND, 7);
WRITE_SHORT(0, 7); // 97
WRITE_SHORT(0, 7); // 99
WRITE_INST(TAILCALL_CONTINUATION, 7); // 100

// put1
WRITE_INST(LIST_NIL, 8);
WRITE_INST(FIND, 8); // 102
WRITE_SHORT(0, 8); // 104
WRITE_SHORT(1, 8); // 106
WRITE_INST(FIND, 8);
WRITE_SHORT(0, 8); // 109
WRITE_SHORT(0, 8); // 111
WRITE_INST(TAILCALL_CONTINUATION, 8); // 112

// after1
WRITE_INST(LIST_NIL, 9);
WRITE_INST(SWAP, 9); // 114
WRITE_INST(LIST_CONS, 9);
WRITE_INST(FIND, 9); // 116
WRITE_SHORT(0, 9); // 118
WRITE_SHORT(0, 9); // 120
WRITE_INST(LIST_CONS, 9);
WRITE_INST(RETURN, 9); // 122

// end
WRITE_INST(ABORT, 15);
WRITE_BYTE(0, 15);

VERIFY_FRAMES(0);
VERIFY_STACK(1);

END_TEST();



BEGIN_TEST("Two nested handle contexts with the same mark id.");

//main =
//  handle {
//    1 raise! 3 add
//  } with {
//    raise! e => handle { 2 resume raise! sub-i32 }
//                with {
//                  raise! x => e x resume
//                  return => 1 add
//                }
//    return => 2 add
//  }

CONSTANT(NUMBER_VAL(2));
CONSTANT(NUMBER_VAL(1));
CONSTANT(NUMBER_VAL(3));

WRITE_INT_INST(CALL, 10, 1); // 5
WRITE_INT_INST(TAILCALL, 118, 2); //10

// main
WRITE_INT_INST(CLOSURE, 110, 3); // 15
WRITE_BYTE(0, 3);
WRITE_SHORT(0, 3); //18

WRITE_INT_INST(CLOSURE, 48, 3); //23
WRITE_BYTE(1, 3);
WRITE_SHORT(0, 3); //26

WRITE_INST(HANDLE, 4);
WRITE_SHORT(12, 4);
WRITE_INT(0, 4);
WRITE_BYTE(0, 4);
WRITE_BYTE(1, 4);

WRITE_INST(CONSTANT, 5);
WRITE_BYTE(1, 5);
WRITE_INT_INST(ESCAPE, 0, 5);
WRITE_BYTE(0, 5);
WRITE_INST(CONSTANT, 5);
WRITE_BYTE(2, 5);
WRITE_INST(ADD, 5);
WRITE_INST(COMPLETE, 5);
WRITE_INST(RETURN, 5);

// raise1
WRITE_INT_INST(CLOSURE, 114, 6);
WRITE_BYTE(0, 6);
WRITE_SHORT(0, 6);

WRITE_INT_INST(CLOSURE, 94, 6);
WRITE_BYTE(1, 6);
WRITE_SHORT(1, 6);
WRITE_SHORT(0, 6);
WRITE_SHORT(1, 6);

WRITE_INST(HANDLE, 7);
WRITE_SHORT(16, 7);
WRITE_INT(0, 7);
WRITE_BYTE(0, 7);
WRITE_BYTE(1, 7);

WRITE_INST(CONSTANT, 8);
WRITE_BYTE(0, 8);
WRITE_INST(FIND, 8);
WRITE_SHORT(1, 8);
WRITE_SHORT(0, 8);
WRITE_INST(CALL_CONTINUATION, 8);
WRITE_INT_INST(ESCAPE, 0, 8);
WRITE_BYTE(0, 8);
WRITE_INST(SUBTRACT, 8);
WRITE_INST(COMPLETE, 8);
WRITE_INST(RETURN, 8);

// raise2
WRITE_INST(FIND, 12);
WRITE_SHORT(0, 12);
WRITE_SHORT(2, 12);
WRITE_INST(FIND, 12); 
WRITE_SHORT(0, 12);
WRITE_SHORT(1, 12);
WRITE_INST(FIND, 12); 
WRITE_SHORT(0, 12);
WRITE_SHORT(0, 12);
WRITE_INST(TAILCALL_CONTINUATION, 12);

// ret1
WRITE_INST(CONSTANT, 13);
WRITE_BYTE(0, 13);
WRITE_INST(ADD, 13);
WRITE_INST(RETURN, 13);

// ret2
WRITE_INST(CONSTANT, 14);
WRITE_BYTE(2, 14);
WRITE_INST(ADD, 14);
WRITE_INST(RETURN, 14);

// end
WRITE_INST(ABORT, 15);
WRITE_BYTE(0, 15);

VERIFY_FRAMES(0);
VERIFY_STACK(1);
VERIFY_NUMBER(9.0);

END_TEST();




BEGIN_TEST("A handler with multiple resumes.");

//main =
//  handle {
//    flip! flip! xor
//  } with {
//    flip! => false resume vars x in { true resume x swap append }
//    return => [] swap cons
//  }

WRITE_INT_INST(CALL, 10, 1); // 5
WRITE_INT_INST(TAILCALL, 79, 2); //10

// main
WRITE_INT_INST(CLOSURE, 75, 3); // 15
WRITE_BYTE(0, 3);
WRITE_SHORT(0, 3); //18

WRITE_INT_INST(CLOSURE, 50, 3); //23
WRITE_BYTE(0, 3);
WRITE_SHORT(0, 3); //26

WRITE_INST(HANDLE, 4);
WRITE_SHORT(14, 4); // 29
WRITE_INT(0, 4); // 33
WRITE_BYTE(0, 4);
WRITE_BYTE(1, 4); // 35

WRITE_INT_INST(ESCAPE, 0, 5); // 40
WRITE_BYTE(0, 5);
WRITE_INT_INST(ESCAPE, 0, 5); // 46
WRITE_BYTE(0, 5);
WRITE_INST(BOOL_NEQ, 5);
WRITE_INST(COMPLETE, 5);
WRITE_INST(RETURN, 5); // 50

// flip
WRITE_INST(FALSE, 6);
WRITE_INST(FIND, 6);
WRITE_SHORT(0, 6); // 54
WRITE_SHORT(0, 6); // 56
WRITE_INST(CALL_CONTINUATION, 6); // 57

WRITE_INST(STORE, 6);
WRITE_BYTE(1, 6);

WRITE_INST(TRUE, 6);
WRITE_INST(FIND, 6); // 61
WRITE_SHORT(1, 6); // 63
WRITE_SHORT(0, 6); // 65
WRITE_INST(CALL_CONTINUATION, 6); // 66

WRITE_INST(FIND, 6);
WRITE_SHORT(0, 6); // 69
WRITE_SHORT(0, 6); // 71
WRITE_INST(SWAP, 6);
WRITE_INST(LIST_APPEND, 6); // 73

WRITE_INST(FORGET, 6);
WRITE_INST(RETURN, 6); // 75

// ret1
WRITE_INST(LIST_NIL, 7);
WRITE_INST(SWAP, 7);
WRITE_INST(LIST_CONS, 7);
WRITE_INST(RETURN, 7); // 79

// end
WRITE_INST(ABORT, 15);
WRITE_BYTE(0, 15);

VERIFY_FRAMES(0);
VERIFY_STACK(1);

END_TEST();