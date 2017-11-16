package kafka.procesor;

import org.apache.storm.topology.base.BaseRichBolt;
import org.apache.storm.topology.OutputFieldsDeclarer;
import org.apache.storm.tuple.Fields;
import org.apache.storm.task.OutputCollector;
import org.apache.storm.task.TopologyContext;
import org.apache.storm.tuple.Tuple;
import org.apache.storm.tuple.Values;

import java.util.Map;

public class GroupingBolt extends BaseRichBolt {

    private OutputCollector collector;

    @Override
    public void prepare(Map stormConf, TopologyContext context,
                        OutputCollector collector) {
        this.collector = collector;
    }

    @Override
    public void execute(Tuple input) {
        String record = input.getString(0);
        String camID=record.split("\"")[3];
        collector.emit(input, new Values(record,camID));
       // collector.emit(input, new Values(record));
        collector.ack(input);
    }

    @Override
    //public void declareOutputFields(OutputFieldsDeclarer declarer) { declarer.declare(new Fields("message"));}
    public void declareOutputFields(OutputFieldsDeclarer declarer) { declarer.declare(new Fields("jsonstaff","cam")); }

}
