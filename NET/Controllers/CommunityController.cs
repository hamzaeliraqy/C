using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Rendering;
using Microsoft.EntityFrameworkCore;
using Lab4.Data;
using Lab4.Models;
using Lab4.Models.ViewModels;

namespace Lab4.Controllers
{
    public class CommunityController : Controller
    {
        private readonly SchoolCommunityContext _context;

        public CommunityController(SchoolCommunityContext context)
        {
            _context = context;
        }

        // GET: Community
        public async Task<IActionResult> Index()
        {
            //was Empty
            // return View(await _context.Communities.ToListAsync());
            var viewModel = new CommunityViewModel();
            return View(await _context.Communities.ToListAsync());
        }

        // GET: Community/Details/5
        public async Task<IActionResult> Details(string id)
        {
            if (id == null)
            {
                return NotFound();
            }

            var community = await _context.Communities
                .FirstOrDefaultAsync(m => m.ID == id);
            if (community == null)
            {
                return NotFound();
            }

            return View(community);
        }

        // GET: Community/Create
        public IActionResult Create()
        {
            return View();
        }

        // POST: Community/Create
        // To protect from overposting attacks, enable the specific properties you want to bind to.
        // For more details, see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Create([Bind("ID,Title,Budget")] Community community)
        {
            if (ModelState.IsValid)
            {
                _context.Add(community);
                await _context.SaveChangesAsync();
                return RedirectToAction(nameof(Index));
            }
            return View(community);
        }

        // GET: Community/Edit/5
        public async Task<IActionResult> Edit(string id)
        {
            if (id == null)
            {
                return NotFound();
            }

            var community = await _context.Communities.FindAsync(id);
            if (community == null)
            {
                return NotFound();
            }
            return View(community);
        }

        // POST: Community/Edit/5
        // To protect from overposting attacks, enable the specific properties you want to bind to.
        // For more details, see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Edit(string id, [Bind("ID,Title,Budget")] Community community)
        {
            if (id != community.ID)
            {
                return NotFound();
            }

            if (ModelState.IsValid)
            {
                try
                {
                    _context.Update(community);
                    await _context.SaveChangesAsync();
                }
                catch (DbUpdateConcurrencyException)
                {
                    if (!CommunityExists(community.ID))
                    {
                        return NotFound();
                    }
                    else
                    {
                        throw;
                    }
                }
                return RedirectToAction(nameof(Index));
            }
            return View(community);
        }

        // GET: Community/Delete/5
        public async Task<IActionResult> Delete(string id)
        {
            if (id == null)
            {
                return NotFound();
            }

            var community = await _context.Communities
                .FirstOrDefaultAsync(m => m.ID == id);
            if (community == null)
            {
                return NotFound();
            }

            return View(community);
        }

        // POST: Community/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> DeleteConfirmed(string id)
        {
            var community = await _context.Communities.FindAsync(id);
            _context.Communities.Remove(community);
            await _context.SaveChangesAsync();
            return RedirectToAction(nameof(Index));
        }

        private bool CommunityExists(string id)
        {
            return _context.Communities.Any(e => e.ID == id);
        }

        public async Task<IActionResult> Select(string id)
        {
            if (id == null)
            {
                return NotFound();
            }

            // var community = await _context.Communities
            //   .FirstOrDefaultAsync(m => m.ID == id);

            var community = await _context.Communities.Include(s => s.CommunityMemberships)
                .ThenInclude(a => a.Student).AsNoTracking()
                .FirstOrDefaultAsync(m => m.ID == id);

            /*var student = await _context.Students.Include(s => s.CommunityMemberships)
                .ThenInclude(a => a.Community).AsNoTracking()
                .FirstOrDefaultAsync(m => m.ID == id);*/
            if (community == null)
            {
                return NotFound();
            }

            return View(community);
        }

        public async Task<IActionResult> Ads(string id, [Bind("FileName,Url")] Ads adsImages)
        {
            /* if (id == null)
             {
                 return NotFound();
             }

             //var community = await _context.Communities.FindAsync(id);
             //var ads = await _context.AdsImages.AddAsync();

             //return RedirectToAction(nameof(Index));
             if (ads == null)
             {
                 return NotFound();
             }*/

            
                _context.Add(adsImages);
                await _context.SaveChangesAsync();
                return RedirectToAction(nameof(Create));
            
            return View("Ads/Create");
        }
    }
}
