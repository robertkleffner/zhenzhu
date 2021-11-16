

BEGIN_TEST("Basic conditional offset test");

CONST_DOUBLE(1);
CONST_DOUBLE(2);

WRITE_INST(TRUE, 1);
WRITE_INST(OFFSET_TRUE, 2);
WRITE_INT(2, 2);
WRITE_INST(ABORT, 2);
WRITE_BYTE(1, 2);
WRITE_INST(CONSTANT, 3);
WRITE_BYTE(0, 3);
WRITE_INST(ABORT, 3);
WRITE_BYTE(0, 3);

VERIFY_STACK(1);
VERIFY_FRAMES(0);
VERIFY_NUMBER(1);

END_TEST();