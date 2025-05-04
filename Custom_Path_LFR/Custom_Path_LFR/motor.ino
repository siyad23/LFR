void motor(int a, int b) {
  if (a >= 0) {
    analogWrite(left_motor_forward, a);
    analogWrite(left_motor_backward, 0);
  } else {
    a = -a;
    analogWrite(left_motor_forward, 0);
    analogWrite(left_motor_backward, a);
  }
  if (b >= 0) {
    analogWrite(right_motor_forward, b);
    analogWrite(right_motor_backward, 0);
  } else {
    b = -b;
    analogWrite(right_motor_forward, 0);
    analogWrite(right_motor_backward, b);
  }
}


void brake() {
  motor(-250, -250);
  delay(brake_time);
  motor(0, 0);
  delay(100);
}

void turn_90(char s) {
  (s == 'l') ? motor(-turn_speed, turn_speed) : motor(turn_speed, -turn_speed);
  delay(turn90_delay);  //this should be adjusted
  (s == 'r') ? motor(-turn_speed, turn_speed) : motor(turn_speed, -turn_speed);
  delay(turn_brake);
  motor(0, 0);
  delay(100);
}

