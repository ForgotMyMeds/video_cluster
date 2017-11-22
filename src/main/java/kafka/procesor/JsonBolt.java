package kafka.procesor;
import java.util.Arrays;
import java.util.Map;
import java.util.ArrayList;
import java.util.List;

import org.apache.storm.generated.AlreadyAliveException;
import org.apache.storm.generated.InvalidTopologyException;
import org.apache.storm.kafka.BrokerHosts;
import org.apache.storm.kafka.KafkaSpout;
import org.apache.storm.kafka.SpoutConfig;
import org.apache.storm.kafka.StringScheme;
import org.apache.storm.kafka.ZkHosts;

import org.apache.storm.shade.org.json.simple.JSONValue;
import org.apache.storm.starter.spout.RandomSentenceSpout;
import org.apache.storm.task.OutputCollector;
import org.apache.storm.task.ShellBolt;
import org.apache.storm.task.TopologyContext;
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
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


/**
 * Created by root on 17-9-29.
 */
public class JsonBolt extends BaseRichBolt{
    private static final Logger LOG = LoggerFactory
            .getLogger(JsonBolt.class);

    private Fields fields;
    private OutputCollector collector;

    public JsonBolt() {
        this.fields = new Fields("hostIp", "instanceName", "className",
                "methodName", "createTime", "callTime", "errorCode");
    }

    @Override
    public void prepare(Map stormConf, TopologyContext context,
                        OutputCollector collector) {
        this.collector = collector;
    }

    @Override
    public void execute(Tuple tuple) {
        String spanDataJson = tuple.getString(0);
        LOG.info("source data:{}", spanDataJson);
        Map<String, Object> map = (Map<String, Object>) JSONValue
                .parse(spanDataJson);
        Values values = new Values();
        for (int i = 0, size = this.fields.size(); i < size; i++) {
            values.add(map.get(this.fields.get(i)));
        }
        this.collector.emit(tuple, values);
        this.collector.ack(tuple);
    }

    @Override
    public void declareOutputFields(OutputFieldsDeclarer declarer) {
        declarer.declare(this.fields);
    }
}