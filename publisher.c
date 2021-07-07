#include "dds/dds.h"
#include <stdio.h>
#include <stdlib.h>

#include <builtin_interfaces/msg/Time.h>
#include <std_msgs/msg/Header.h>
#include <sensor_msgs/msg/JointState.h>

int main(int argc, char **argv)
{
  dds_entity_t participant;
  dds_entity_t topic;
  dds_entity_t publisher;
  dds_entity_t writer;
  dds_return_t rc;

  builtin_interfaces_msg_Time *time;
  std_msgs_msg_Header *header;
  sensor_msgs_msg_JointState *msg;

  uint32_t status = 0;
  (void)argc;
  (void)argv;

  /* Create a Participant. */
  participant = dds_create_participant(DDS_DOMAIN_DEFAULT, NULL, NULL);
  if (participant < 0)
    DDS_FATAL("dds_create_participant: %s\n", dds_strretcode(-participant));

  /* Create a Topic. */
  topic = dds_create_topic(
      participant, &sensor_msgs_msg_JointState_desc, "rt/joint_states", NULL, NULL);
  if (topic < 0)
    DDS_FATAL("dds_create_topic: %s\n", dds_strretcode(-topic));

  /* Create a Writer. */
  writer = dds_create_writer(participant, topic, NULL, NULL);
  if (writer < 0)
    DDS_FATAL("dds_create_writer: %s\n", dds_strretcode(-writer));

  printf("=== [Publisher]  Waiting for a reader to be discovered ...\n");
  fflush(stdout);

  rc = dds_set_status_mask(writer, DDS_PUBLICATION_MATCHED_STATUS);
  if (rc != DDS_RETCODE_OK)
    DDS_FATAL("dds_set_status_mask: %s\n", dds_strretcode(-rc));

  while (!(status & DDS_PUBLICATION_MATCHED_STATUS))
  {
    rc = dds_get_status_changes(writer, &status);
    if (rc != DDS_RETCODE_OK)
      DDS_FATAL("dds_get_status_changes: %s\n", dds_strretcode(-rc));

    /* Polling sleep. */
    dds_sleepfor(DDS_MSECS(20));
  }

  time = builtin_interfaces_msg_Time__alloc();
  time->sec = 123;
  time->nanosec = 456;

  builtin_interfaces_msg_Time time2;
  time2.sec = 3434;
  time2.nanosec = 232;

  header = std_msgs_msg_Header__alloc();
  header->frame_id = "";

  msg = sensor_msgs_msg_JointState__alloc();

  msg->header.frame_id = "iiwa";
  msg->header.stamp = time2;

  uint32_t num_joints = 7;

  dds_sequence_string name;
  name._buffer = dds_sequence_string_allocbuf(num_joints);
  name._length = num_joints;
  name._buffer[0] = "joint_a1";
  name._buffer[1] = "joint_a2";
  name._buffer[2] = "joint_a3";
  name._buffer[3] = "joint_a4";
  name._buffer[4] = "joint_a5";
  name._buffer[5] = "joint_a6";
  name._buffer[6] = "joint_a7";

  dds_sequence_double position;
  position._buffer = dds_sequence_double_allocbuf(num_joints);
  position._length = num_joints;
  position._release = true;
  for (uint32_t i = 0; i < num_joints; i++)
  {
    position._buffer[i] = (double)i + 1.0;
  }

  msg->name = name;
  msg->position = position;

  sensor_msgs_msg_JointState msg2;
  msg2.header = *std_msgs_msg_Header__alloc();
  msg2.name = *dds_sequence_string__alloc();
  msg2.position = *dds_sequence_double__alloc();
  msg2.velocity = *dds_sequence_double__alloc();
  msg2.effort = *dds_sequence_double__alloc();

  printf("=== [Publisher]  Writing : ");
  // printf ("Message (%"PRId32")\n", msg.data);
  fflush(stdout);

  rc = dds_write(writer, msg);
  if (rc != DDS_RETCODE_OK)
    DDS_FATAL("dds_write: %s\n", dds_strretcode(-rc));

  rc = dds_write(writer, &msg2);
  if (rc != DDS_RETCODE_OK)
    DDS_FATAL("dds_write: %s\n", dds_strretcode(-rc));

  /* Deleting the participant will delete all its children recursively as well. */
  rc = dds_delete(participant);
  if (rc != DDS_RETCODE_OK)
    DDS_FATAL("dds_delete: %s\n", dds_strretcode(-rc));

  return EXIT_SUCCESS;
}
