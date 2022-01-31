package wireshark;


import org.apache.commons.lang3.RandomStringUtils;
import org.apache.rocketmq.client.exception.MQBrokerException;
import org.apache.rocketmq.client.exception.MQClientException;
import org.apache.rocketmq.client.producer.DefaultMQProducer;
import org.apache.rocketmq.client.producer.SendResult;
import org.apache.rocketmq.common.message.Message;
import org.apache.rocketmq.remoting.exception.RemotingException;

import java.util.concurrent.TimeUnit;

/**
 * Created By Arthur Zhang at 23/05/2017
 */
interface MQService {
    void start();

    void enqueue(String topic, String tags, byte[] data);

    void shutdown();
}

public class RocketMQServiceImpl implements MQService {
    public static final String MQ_CONSUMER_GROUP_NAME = "ya_test_group";
    public static final String MQ_PRODUCER_GROUP_NAME = "ya_test_produce_group";

    public DefaultMQProducer producer;

    public RocketMQServiceImpl() {
        producer = new DefaultMQProducer(MQ_PRODUCER_GROUP_NAME);
        producer.setNamesrvAddr("localhost:9876");
    }

    @Override
    public void start() {
        try {
            producer.start();
        } catch (MQClientException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void enqueue(String topic, String tags, byte[] data) {

        Message message = new Message(topic, tags, data);
        SendResult sendResult;
        try {
            sendResult = producer.send(message);
            System.out.println(sendResult);
        } catch (MQClientException | RemotingException | InterruptedException | MQBrokerException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void shutdown() {
        producer.shutdown();
    }


    public static void main(String[] args) throws InterruptedException, MQBrokerException, RemotingException, MQClientException {

        RocketMQServiceImpl service = new RocketMQServiceImpl();
        service.start();

        final DefaultMQProducer producer = service.producer;

        final String topic = "ya_test_message";
        final String tags = "xx";


        for (int i = 0; i < 10000; i++) {
            String s = RandomStringUtils.randomNumeric(20);
            System.out.println("i:" + i + ">" + s);
            byte[] data = (">>>" + s).getBytes();
            final Message message = new Message(topic, tags, data);

            producer.send(message);
            TimeUnit.SECONDS.sleep(1);
        }
    }
}
