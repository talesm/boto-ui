const fs = require('fs')

const fileQueue = makeQueue('dui.hpp')

const output = fs.openSync('dui_single.hpp', 'w')
fs.writeSync(output, "/*\n * ", undefined)
const copywrite = fs.readFileSync('LICENSE', 'utf-8')
fs.writeSync(output, copywrite.replace(/\n/g, '\n * '))
fs.writeSync(output, "\n */\n", undefined)
fs.writeSync(output, "#ifndef DUI_SINGLE_HPP\n", undefined)
fs.writeSync(output, "#define DUI_SINGLE_HPP\n\n", undefined)
fs.writeSync(output, "#include <algorithm>\n", undefined)
fs.writeSync(output, "#include <string>\n", undefined)
fs.writeSync(output, "#include <string_view>\n", undefined)
fs.writeSync(output, "#include <vector>\n", undefined)
fs.writeSync(output, "#include <SDL.h>\n\n", undefined)
fs.writeSync(output, "namespace dui {\n\n", undefined)
fs.writeSync(output, "#ifndef DUI_THEME\n", undefined)
fs.writeSync(output, "#define DUI_THEME dui::style::SteelBlue\n", undefined)
fs.writeSync(output, "#endif\n\n", undefined)

for (const fileName of fileQueue) {
  fs.writeSync(output, `// begin ${fileName}\n`)
  const content = fs.readFileSync(fileName, 'utf-8')
  fs.writeSync(output, content
    .replace(/^#.*$/gm, '')
    .replace(/^namespace dui \{$/gm, '')
    .replace(/^\} \/\/ namespace dui$/gm, '')
    .trim()
  )
  fs.writeSync(output, `\n\n`)

}

fs.writeSync(output, "} // namespace dui\n\n", undefined)
fs.writeSync(output, "#endif // DUI_SINGLE_HPP\n", undefined)


/**
 * 
 * @param {string} file
 * @param {string[]|undefined} queue
 */
function makeQueue(file, queue) {
  queue = queue || []
  const fileContent = fs.readFileSync(file, 'utf-8')
  const dependencies = getDependencies(fileContent)
  while (dependencies.length > 0) {
    const dependency = dependencies.shift()
    if (queue.indexOf(dependency) !== -1) {
      continue
    }
    makeQueue(dependency, queue)
  }
  if (queue.indexOf(file) !== -1) {
    throw new Error(`File ${file} is in loop`)
  }
  queue.push(file)
  return queue
}

/**
 * 
 * @param {string} content 
 */
function getDependencies(content) {
  const result = []
  const regexp = /^#include "(.*)"$/m
  let m = content.match(regexp)
  while (m) {
    result.push(m[1])
    content = content.slice(m.index + m[0].length)
    m = content.match(regexp)
  }
  return result
}
