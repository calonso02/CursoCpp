<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Grupo Los Leones - Proyecto C++</title>
    <script src="https://cdn.tailwindcss.com"></script>
    <link href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css" rel="stylesheet">
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@300;400;600;700&family=JetBrains+Mono:wght@400;700&display=swap" rel="stylesheet">
    <style>
        body {
            font-family: 'Inter', sans-serif;
            background-color: #0f172a; /* Slate 900 */
            background-image: radial-gradient(at 0% 0%, hsla(253,16%,7%,1) 0, transparent 50%), radial-gradient(at 50% 0%, hsla(225,39%,30%,1) 0, transparent 50%), radial-gradient(at 100% 0%, hsla(339,49%,30%,1) 0, transparent 50%);
        }
        .mono-font {
            font-family: 'JetBrains Mono', monospace;
        }
        .glass-card {
            background: rgba(30, 41, 59, 0.7);
            backdrop-filter: blur(10px);
            border: 1px solid rgba(255, 255, 255, 0.1);
        }
    </style>
</head>
<body class="text-slate-200 min-h-screen flex flex-col items-center py-12 px-4">

    <!-- Contenedor Principal -->
    <main class="w-full max-w-3xl space-y-8">

        <!-- Encabezado -->
        <header class="text-center space-y-4 animate-fade-in-down">
            <div class="inline-block p-3 rounded-full bg-blue-600/20 text-blue-400 mb-2 border border-blue-500/30">
                <i class="fa-brands fa-cuttlefish text-2xl"></i>
                <span class="ml-2 font-bold tracking-wide">PROYECTO C++</span>
            </div>
            <h1 class="text-5xl font-bold text-white tracking-tight">
                Grupo <span class="text-transparent bg-clip-text bg-gradient-to-r from-blue-400 to-emerald-400">Los Leones</span>
            </h1>
            <p class="text-slate-400 text-lg max-w-xl mx-auto">
                Repositorio oficial del proyecto de ingeniería.
            </p>
        </header>

        <!-- Sección de Integrantes -->
        <section class="glass-card rounded-2xl p-8 shadow-2xl">
            <h2 class="text-xl font-semibold text-white mb-6 flex items-center">
                <i class="fa-solid fa-users text-emerald-400 mr-3"></i> Integrantes del Equipo
            </h2>
            
            <div class="grid grid-cols-1 md:grid-cols-2 gap-4">
                <!-- Integrante 1 -->
                <div class="flex items-center p-3 rounded-lg bg-slate-800/50 hover:bg-slate-800 transition-colors border border-slate-700/50">
                    <div class="h-10 w-10 rounded-full bg-gradient-to-br from-blue-500 to-purple-600 flex items-center justify-center text-white font-bold text-sm">JG</div>
                    <span class="ml-3 font-medium text-slate-200">Juan Francisco Gonzalez Arenas</span>
                </div>

                <!-- Integrante 2 -->
                <div class="flex items-center p-3 rounded-lg bg-slate-800/50 hover:bg-slate-800 transition-colors border border-slate-700/50">
                    <div class="h-10 w-10 rounded-full bg-gradient-to-br from-emerald-500 to-teal-600 flex items-center justify-center text-white font-bold text-sm">ML</div>
                    <span class="ml-3 font-medium text-slate-200">Miguel López Cordero</span>
                </div>

                <!-- Integrante 3 -->
                <div class="flex items-center p-3 rounded-lg bg-slate-800/50 hover:bg-slate-800 transition-colors border border-slate-700/50">
                    <div class="h-10 w-10 rounded-full bg-gradient-to-br from-orange-500 to-red-600 flex items-center justify-center text-white font-bold text-sm">CL</div>
                    <span class="ml-3 font-medium text-slate-200">Carlos Laguna Tellez</span>
                </div>

                <!-- Integrante 4 -->
                <div class="flex items-center p-3 rounded-lg bg-slate-800/50 hover:bg-slate-800 transition-colors border border-slate-700/50">
                    <div class="h-10 w-10 rounded-full bg-gradient-to-br from-indigo-500 to-violet-600 flex items-center justify-center text-white font-bold text-sm">CA</div>
                    <span class="ml-3 font-medium text-slate-200">Cándido Alonso Barro</span>
                </div>
            </div>
        </section>

        <!-- Sección de Video Demo -->
        <section class="glass-card rounded-2xl overflow-hidden shadow-2xl border border-slate-700/50">
            <div class="p-4 border-b border-slate-700/50 bg-slate-800/30 flex justify-between items-center">
                <h2 class="text-lg font-semibold text-white flex items-center">
                    <i class="fa-brands fa-youtube text-red-500 mr-3"></i> Demo del Proyecto
                </h2>
                <span class="text-xs font-mono text-slate-500 bg-slate-900 px-2 py-1 rounded">VIDEO_DEMO</span>
            </div>
            <div class="aspect-video w-full bg-black relative">
                <!-- Iframe de YouTube -->
                <iframe 
                    class="w-full h-full"
                    src="https://www.youtube.com/embed/v9ztH5WzsEM" 
                    title="YouTube video player" 
                    frameborder="0" 
                    allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" 
                    allowfullscreen>
                </iframe>
            </div>
        </section>

        <!-- Footer -->
        <footer class="text-center pt-8 border-t border-slate-800/50">
            <p class="text-slate-500 text-sm mono-font">
                &copy; 2025 Grupo Los Leones. <span class="text-slate-600">|</span> Ingeniería Aeroespacial
            </p>
        </footer>

    </main>

</body>
</html>
