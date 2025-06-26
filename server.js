const express = require('express');
const { spawn } = require('child_process');

const app = express();
app.use(express.json());
app.use(express.static('.'));  

app.post('/api/compile', (req, res) => {
    const code = req.body.code;
    console.log('Received code:', code);

    const executable = process.platform === 'win32' ? 'mini_compile.exe' : './mini_compile';

    const child = spawn(executable);

    let output = '';
    let errorOutput = '';

    child.stdout.on('data', (data) => {
        output += data.toString();
    });

    child.stderr.on('data', (data) => {
        errorOutput += data.toString();
    });

    child.on('error', (err) => {
        console.error('Failed to start child process:', err);
        res.status(500).json({ error: 'Server error: failed to run compiler' });
    });

    child.on('close', (code) => {
        console.log(`Child process exited with code ${code}`);
        console.log('Compiler stdout:', output);
        console.log('Compiler stderr:', errorOutput);

        
        res.json({
            output: output.trim(),
            stderr: errorOutput.trim()
        });
    });

    child.stdin.write(code);
    child.stdin.end();
});

const PORT = 3000;
app.listen(PORT, () => {
    console.log(`Server running at http://localhost:${PORT}`);
});
