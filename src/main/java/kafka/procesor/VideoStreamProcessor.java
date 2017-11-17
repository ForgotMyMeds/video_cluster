package kafka.procesor;
import java.util.*;

import org.apache.storm.generated.AlreadyAliveException;
import org.apache.storm.generated.InvalidTopologyException;
import org.apache.storm.kafka.BrokerHosts;
import org.apache.storm.kafka.KafkaSpout;
import org.apache.storm.kafka.SpoutConfig;
import org.apache.storm.kafka.StringScheme;
import org.apache.storm.kafka.ZkHosts;
import org.apache.storm.kafka.bolt.KafkaBolt;
import org.apache.storm.starter.spout.RandomSentenceSpout;
import org.apache.storm.task.ShellBolt;
import org.apache.storm.topology.BasicOutputCollector;
import org.apache.storm.topology.IRichBolt;
import org.apache.storm.topology.OutputFieldsDeclarer;
import org.apache.storm.topology.TopologyBuilder;
import org.apache.storm.topology.base.BaseRichBolt;
import org.apache.storm.tuple.Fields;
import org.apache.storm.tuple.Tuple;
import org.apache.storm.tuple.Values;
import org.apache.storm.Config;
import org.apache.storm.LocalCluster;
import org.apache.storm.StormSubmitter;
import org.apache.storm.spout.SchemeAsMultiScheme;
/**
 * Created by root on 17-9-29.
 */
public class VideoStreamProcessor {
    private static final String TOPOLOGY_NAME = "SPAN-DATA-TOPOLOGY";
    private static final String KAFKA_SPOUT_ID = "kafka-stream";
    private static final String JsonProject_BOLT_ID = "mytest-bolt";

    public static void main(String[] args) throws Exception {
        String zks = "129.168.6.53:2181";
        String topic = "davi";
        String zkRoot = "/kafka-stm";
        BrokerHosts brokerHosts = new ZkHosts(zks);
        SpoutConfig spoutConf = new SpoutConfig(brokerHosts, topic, zkRoot,
                KAFKA_SPOUT_ID);
        spoutConf.scheme = new SchemeAsMultiScheme(new StringScheme());
        spoutConf.zkServers = Arrays.asList(new String[] { "129.168.6.53" });
        spoutConf.zkPort = 2181;
        //spoutConf.startOffsetTime = -1;
        //JsonBolt jsonBolt = new JsonBolt();
        PicProcessBolt testBolt = new PicProcessBolt();
        GroupingBolt groupBolt= new GroupingBolt();

        Properties prop=new Properties();
        prop.put("bootstrap.servers","129.168.6.53:9092");
        prop.put("key.serializer","org.apache.kafka.common.serialization.StringSerializer");
        prop.put("value.serializer","org.apache.kafka.common.serialization.StringSerializer");
        prop.put("max.request.size","1300000");
        prop.put("compression.type","gzip");
       // prop.put("metadata.broker.list","172.17.0.2:9092");
       // prop.put("topic","output");
        KafkaBolt kafkabolt = new KafkaBolt().withProducerProperties(prop);

        TopologyBuilder builder = new TopologyBuilder();
        builder.setSpout(KAFKA_SPOUT_ID, new KafkaSpout(spoutConf));
        builder.setBolt("grouping-bolt", groupBolt).shuffleGrouping(KAFKA_SPOUT_ID);
        builder.setBolt(JsonProject_BOLT_ID, testBolt).fieldsGrouping("grouping-bolt", new Fields("cam"));
        builder.setBolt("kafkabolt", kafkabolt).shuffleGrouping(JsonProject_BOLT_ID);
        Config config = new Config();
        Map<String, String> map = new HashMap<String, String>();
        map.put("metadata.broker.list", "129.168.6.53:9092");
        // serializer.class为消息的序列化类
        map.put("serializer.class", "org.apache.kafka.serializer.StringEncoder");
        config.put("kafka.broker.properties", map);
        config.put("topic", "output1");
        config.setNumWorkers(1);
        if (args.length == 0) {
            LocalCluster cluster = new LocalCluster();
            cluster.submitTopology(TOPOLOGY_NAME, config,
                    builder.createTopology());
          //  Utils.waitForSeconds(100);
            cluster.killTopology(TOPOLOGY_NAME);
            cluster.shutdown();
        } else {
            StormSubmitter.submitTopology(args[0], config,
                    builder.createTopology());
        }
    }
}
