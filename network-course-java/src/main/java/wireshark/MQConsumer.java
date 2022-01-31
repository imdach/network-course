package wireshark;

import org.apache.rocketmq.client.consumer.DefaultMQPushConsumer;
import org.apache.rocketmq.client.consumer.listener.ConsumeConcurrentlyStatus;
import org.apache.rocketmq.client.consumer.listener.MessageListenerConcurrently;
import org.apache.rocketmq.client.exception.MQClientException;
import org.apache.rocketmq.common.consumer.ConsumeFromWhere;
import org.apache.rocketmq.common.message.Message;

/**
 * Created By Arthur Zhang at 23/05/2017
 */
public class MQConsumer {
    public static final String MQ_CONSUMER_GROUP_NAME = "ya_test_group";

    public void start() throws MQClientException {
        DefaultMQPushConsumer consumer = new DefaultMQPushConsumer(MQ_CONSUMER_GROUP_NAME);
//        consumer.setNamesrvAddr("sr-dev-rocketmq-1.gz.cvte.cn:9876;sr-dev-rocketmq-2.gz.cvte.cn:9876");
        consumer.setNamesrvAddr("localhost:9876");
        consumer.setConsumeFromWhere(ConsumeFromWhere.CONSUME_FROM_LAST_OFFSET);
        consumer.subscribe("ya_test_message", "");

        consumer.registerMessageListener((MessageListenerConcurrently) (msgs, context) -> {

            System.out.println("......");
            if (msgs == null || msgs.isEmpty()) {
                return null;
            }
            Message m = msgs.get(0);
            if (m.getBody() == null) {
                return null;
            }
            String str = new String(m.getBody());
            System.out.println(str);
            return ConsumeConcurrentlyStatus.CONSUME_SUCCESS;
        });
        consumer.start();
    }

    public static void main(String[] args) throws MQClientException {
        new MQConsumer().start();
    }
}
