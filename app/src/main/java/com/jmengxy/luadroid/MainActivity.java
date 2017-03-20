package com.jmengxy.luadroid;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Pair;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.jmengxy.lualib.Lua;

public class MainActivity extends AppCompatActivity {

    public static final String INPUT = "input";
    public static final String OUTPUT = "output";

    private EditText inputEdit;
    private EditText scriptEdit;
    private TextView outputText;
    private Button executeButton;

    private Lua lua;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        initUI();
        initLua();
    }

    private void initLua() {
        lua = new Lua();
        executeButton.setOnClickListener(v -> {
            lua.setString(INPUT, inputEdit.getText().toString());
            Pair<Boolean, String> result = lua.parseLine(scriptEdit.getText().toString());
            if (result.first) {
                String outputStr = "";
                switch (lua.getType(OUTPUT)) {
                    case Lua.LUA_TYPE_NIL:
                        outputStr = "nil";
                        break;
                    case Lua.LUA_TYPE_NUMBER:
                        outputStr = lua.isInteger(OUTPUT)
                                ? Integer.toString(lua.getInteger(OUTPUT, 0))
                                : Double.toString(lua.getDouble(OUTPUT, 0.0));
                        break;
                    case Lua.LUA_TYPE_BOOLEAN:
                        outputStr = lua.getBoolean(OUTPUT, false) ? "true" : "false";
                        break;
                    case Lua.LUA_TYPE_STRING:
                        outputStr = lua.getString(OUTPUT, "");
                        break;
                    default:
                        outputStr = "";
                        break;
                }
                outputText.setText(outputStr);
            } else {
                outputText.setText("");
                Toast.makeText(this, "Script parse failed!", Toast.LENGTH_SHORT).show();
            }
        });
    }

    private void initUI() {
        setContentView(R.layout.activity_main);
        inputEdit = (EditText)findViewById(R.id.input);
        scriptEdit = (EditText)findViewById(R.id.script);
        outputText = (TextView) findViewById(R.id.output);
        executeButton = (Button) findViewById(R.id.execute);
    }

    @Override
    protected void onDestroy() {
        if (lua != null) {
            lua.close();
            lua = null;
        }
        super.onDestroy();
    }
}
