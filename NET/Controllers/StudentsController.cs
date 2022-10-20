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
    public class StudentsController : Controller
    {
        private readonly SchoolCommunityContext _context;

        public StudentsController(SchoolCommunityContext context)
        {
            _context = context;
        }

        // GET: Students
        public async Task<IActionResult> Index()
        {
            return View(await _context.Students.ToListAsync());
        }

        // GET: Students/Details/5
        public async Task<IActionResult> Details(int? id)
        {
            if (id == null)
            {
                return NotFound();
            }

            var student = await _context.Students
                .FirstOrDefaultAsync(m => m.ID == id);
            if (student == null)
            {
                return NotFound();
            }

            return View(student);
        }

        // GET: Students/Create
        public IActionResult Create()
        {
            return View();
        }

        // POST: Students/Create
        // To protect from overposting attacks, enable the specific properties you want to bind to.
        // For more details, see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Create([Bind("ID,LastName,FirstName,EnrollmentDate")] Student student)
        {
            if (ModelState.IsValid)
            {
                _context.Add(student);
                await _context.SaveChangesAsync();
                return RedirectToAction(nameof(Index));
            }
            return View(student);
        }

        // GET: Students/Edit/5
        public async Task<IActionResult> Edit(int? id)
        {
            if (id == null)
            {
                return NotFound();
            }

            var student = await _context.Students.FindAsync(id);
            if (student == null)
            {
                return NotFound();
            }
            return View(student);
        }

        // POST: Students/Edit/5
        // To protect from overposting attacks, enable the specific properties you want to bind to.
        // For more details, see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Edit(int id, [Bind("ID,LastName,FirstName,EnrollmentDate")] Student student)
        {
            if (id != student.ID)
            {
                return NotFound();
            }

            if (ModelState.IsValid)
            {
                try
                {
                    _context.Update(student);
                    await _context.SaveChangesAsync();
                }
                catch (DbUpdateConcurrencyException)
                {
                    if (!StudentExists(student.ID))
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
            return View(student);
        }

        // GET: Students/Delete/5
        public async Task<IActionResult> Delete(int? id)
        {
            if (id == null)
            {
                return NotFound();
            }

            var student = await _context.Students
                .FirstOrDefaultAsync(m => m.ID == id);
            if (student == null)
            {
                return NotFound();
            }

            return View(student);
        }

        // POST: Students/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> DeleteConfirmed(int id)
        {
            var student = await _context.Students.FindAsync(id);
            _context.Students.Remove(student);
            await _context.SaveChangesAsync();
            return RedirectToAction(nameof(Index));
        }

        private bool StudentExists(int id)
        {
            return _context.Students.Any(e => e.ID == id);
        }

        /*[HttpPost]
        [ValidateAntiForgeryToken]*/
        public async Task<IActionResult> Select(int? id)
        {
            if (id == null)
            {
                return NotFound();
            }

            /* string comm = null;

             var student = await _context.Students.FirstOrDefaultAsync(i => i.ID == id);
             var communities = await _context.CommunityMemberships.FirstOrDefaultAsync(student => student.CommunityID == comm);*/

            /* var student = await _context.Students
                 .FirstOrDefaultAsync(m => m.ID == id);*/



            //var com = await _context.Communities.FirstOrDefaultAsync(a => a.Title);

            // var commu = await _context.Communities.ToListAsync(id => id.Membership == comID);

            //var student = await _context.Students.FirstOrDefaultAsync(i => i.ID == id);

            var student = await _context.Students.Include(s => s.CommunityMemberships)
                .ThenInclude(a => a.Community).AsNoTracking()
                .FirstOrDefaultAsync(m => m.ID == id);
            //var commu = await _context.CommunityMemberships.FindAsync(id);
            //var studentt = await _context.CommunityMemberships.FindAsync(id.ToString());

            // var comm = new Community();

            //var ha = await _context.Students.Include(a => a.Memberships).FirstOrDefaultAsync(i => i.ID == id);

            //var ha = CommunityMembership.(student, comm.Membership);

            /*var ha = await _context.Students.Include(i => i.Membership)
                .OrderBy(i => i.ID).ToListAsync();

           // var comm = await _context.CommunityMemberships.All();

            var output = await _context.CommunityMemberships.FindAsync(student);
            


            /*var student = await _context.Students
                .FirstOrDefaultAsync(m => m.ID == id);*/ //get student based on id


            //var output = await _context.CommunityMemberships.FirstOrDefaultAsync(CommunityMembership.Equals(id, comm));

            //var student = new StudentMembershipViewModel();

            //var communities = new StudentMembershipViewModel();



            //var Students = await _context.CommunityMemberships.Contains(i => i.Students);

            //CommunityMembership[] comm = new CommunityMembership;



            /* var student = await _context.Students
                 .FirstOrDefaultAsync(m => m.ID == id);*/

            /*var student = await _context.CommunityMemberships
                .FirstOrDefaultAsync(m => m.Stud == id); //community

            /* for()

             var comm = await _context.CommunityMemberships[student, Community];
            */
            if (student == null)
            {
                return NotFound();
            }

            return View(student);
        }

        public async Task<IActionResult> AddMembership(int? id, string[] memb)
        {

           /* var student = await _context.Students.Include(s => s.CommunityMemberships)
                .ThenInclude(a => a.Community).AsNoTracking()
                .FirstOrDefaultAsync(m => m.ID == id);

            var community = await _context.Communities
                .FirstOrDefaultAsync(m => m.ID == cid);*/

            if(id == null)
            {
                return NotFound();
            }

            var studentToUpdate = await _context.Students
                .Include(i => i.CommunityMemberships)
                .ThenInclude(i => i.Community)
                .FirstOrDefaultAsync(m => m.ID == id);

            if(await TryUpdateModelAsync<Student>(
                    studentToUpdate,"", 
                    i => i.FullName))
                    {
               /* if(string.IsNullOrWhiteSpace(studentToUpdate.Communities.Contains))
                {
                    studentToUpdate.CommunityMemberships = null;
                }*/
                UpdateStudentMemberships(memb, studentToUpdate);
                try
                {
                    await _context.SaveChangesAsync();
                }
                catch (DbUpdateException /* ex */)
                {

                }
                return RedirectToAction(nameof(Index));
            }
            UpdateStudentMemberships(memb, studentToUpdate);
            PopulateAssignedMembershipData(studentToUpdate);
            
            
            return View(studentToUpdate);

            

           
        }

        private void UpdateStudentMemberships(string[] memb, Student studentToUpdate)
        {
            if(memb == null)
            {
                studentToUpdate.CommunityMemberships = new List<CommunityMembership>();
                return;
            }

            var selectedCourses = new HashSet<String>(memb);
            var studentMemberships = new HashSet<string>
                (studentToUpdate.CommunityMemberships.Select(c => c.CommunityID));
            foreach(var communities in _context.Communities)
            {
                if (selectedCourses.Contains(communities.ID.ToString()))
                {
                    if (!studentMemberships.Contains(communities.ID)){
                        studentToUpdate.CommunityMemberships.Add(new CommunityMembership { StudentID = studentToUpdate.ID, CommunityID = communities.ID });

                    }

                    }
                else
                {
                    if (studentMemberships.Contains(communities.ID))
                    {
                        CommunityMembership membershipToRemove = studentToUpdate.CommunityMemberships.FirstOrDefault(i => i.CommunityID == communities.ID);
                        _context.Remove(membershipToRemove);
                    }
                }
                
                }
            }

        public void PopulateAssignedMembershipData(Student student)
        {
            var allMmeberships = _context.Communities;
            var studentMemberships = new HashSet<string>(student.CommunityMemberships.Select(c => c.CommunityID));
            var viewModel = new List<CommunityMembership>();
            foreach (var membership in allMmeberships)
            {
                viewModel.Add(new CommunityMembership
                {
                    CommunityID = membership.ID,
                });
                ViewData["Communities"] = viewModel;
                

            }
        }
    }
}

