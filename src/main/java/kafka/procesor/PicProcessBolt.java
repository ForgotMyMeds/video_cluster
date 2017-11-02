package kafka.procesor;

import org.apache.storm.task.ShellBolt;
import org.apache.storm.topology.IRichBolt;
import org.apache.storm.topology.OutputFieldsDeclarer;
import org.apache.storm.tuple.Fields;

import java.util.Map;

/**
 * Created by root on 17-10-15.
 */
public class PicProcessBolt  extends ShellBolt implements IRichBolt {
    public PicProcessBolt()
    {

        super("/bin/sh","im_proc.sh");
    }

    @Override
    public void declareOutputFields(OutputFieldsDeclarer declarer) {
        declarer.declare(new Fields("image"));

    }

    @Override
    public Map<String, Object> getComponentConfiguration() {

        return null;
    }
}
